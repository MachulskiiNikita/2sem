#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

class Chat
{
private:
    using shared_memory_t = boost::interprocess::managed_shared_memory;
    using manager_t = shared_memory_t::segment_manager;
    using string_allocator_t = boost::interprocess::allocator < char, manager_t >;
    using string_t = boost::interprocess::basic_string < char, std::char_traits < char >, string_allocator_t>;
    using vector_allocator_t = boost::interprocess::allocator < string_t, manager_t >;
    using vector_t = boost::interprocess::vector<string_t, vector_allocator_t>;
    using mutex_t = boost::interprocess::interprocess_mutex;
    using condition_t = boost::interprocess::interprocess_condition;
    using counter_t = std::atomic < std::size_t >;

public:
    explicit Chat(const std::string& user_name) : m_user_name(user_name), m_exit_flag(false),
        m_shared_memory(boost::interprocess::open_or_create, shared_memory_name.c_str(), 1024 * 10)
    {
        m_vector = m_shared_memory.find_or_construct < vector_t >("vector")(m_shared_memory.get_segment_manager());
        m_mutex = m_shared_memory.find_or_construct<mutex_t>(mutex_name.c_str())();
        m_condition = m_shared_memory.find_or_construct<condition_t>(condition_name.c_str())();
        m_users = m_shared_memory.find_or_construct <counter_t>("amount_of_users")(1);
        m_messages = m_shared_memory.find_or_construct <counter_t>("amount_of_messages")(0);
        m_local_messages = *m_messages;
    }

    ~Chat() noexcept = default;

public:

    void run()
    {
        auto reader = std::thread(&Chat::read, this);
        write();
        send_message("(" + m_user_name + ") has left the chat");
        reader.join();
        if (!(--(*m_users)))
        {
            boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());
        }
    }

private:

    void read()
    {
        show_history();
        send_message("user (" + m_user_name + ") has joined the chat");

        while (true)
        {
            std::unique_lock lock(*m_mutex);
            m_condition->wait(lock, [this]() { return  m_exit_flag || (m_local_messages < *m_messages); });

            if (m_exit_flag)
            {
                break;
            }

            for (auto i = m_local_messages; i < *m_messages; i++)
            {
                std::cout << (*m_vector)[i] << std::endl;
            }
            m_local_messages = *m_messages;
        }
    }

    void show_history()
    {
        std::unique_lock lock(*m_mutex);
        for (const auto& message : *m_vector)
        {
            std::cout << message << std::endl;
        }
    }

    void send_message(const std::string& message)
    {
        std::unique_lock lock(*m_mutex);
        m_vector->push_back(string_t(message.c_str(), m_shared_memory.get_segment_manager()));
        m_local_messages++;
        (*m_messages)++;
        m_condition->notify_all();
    }

    void write()
    {
        std::string message;
        while (true)
        {
            std::getline(std::cin, message);
            if (message == "QUIT")
            {
                m_exit_flag = true;
                break;
            }
            send_message("(" + m_user_name + "):   " + message);
        }
    }


private:

    static inline const std::string shared_memory_name = "shared_memory";
    static inline const std::string mutex_name = "mutex";
    static inline const std::string condition_name = "condition";

private:

    std::string m_user_name;

    std::atomic < bool > m_exit_flag;

    shared_memory_t m_shared_memory;

    vector_t* m_vector;
    mutex_t* m_mutex;
    condition_t* m_condition;
    counter_t* m_users;
    counter_t* m_messages;

    std::size_t m_local_messages;
};

int main(int argc, char** argv)
{
    std::string user_name;

    std::cout << "Write QUIT to left the chat\nEnter your name: ";

    std::getline(std::cin, user_name);

    Chat(user_name).run();

    system("pause");

    return EXIT_SUCCESS;
}