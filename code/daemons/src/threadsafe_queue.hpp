#ifndef __THREADSAFEQUEUE_HPP__
#define __THREADSAFEQUEUE_HPP__

#include <memory> 
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut; 
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){};
    threadsafe_queue(const threadsafe_queue&) {
         std::lock_guard<std::mutex> lk(other.mut);
         data_queue=other.data_queue;
    }
    threadsafe_queue& operator=(const threadsafe_queue&) = delete;//禁止赋值操作是为了简化 
    void push(T new_value) {
         std::lock_guard<std::mutex> lk(mut);
         data_queue.push(new_value);
         data_cond.notify_one();
    }
    bool try_pop(T& value) { //尝试删除队首元素，若删除成功则通过value返回队首元素，并返回true;若队为空，则返回false 
         std::lock_guard<std::mutex> lk(mut);
         if(data_queue.empty())
             return false;
         value=data_queue.front();
         data_queue.pop();
         return true;
    }
    std::shared_ptr<T> try_pop() { //若队非空shared_ptr返回并删除的队首元素;若队空，则返回的shared_ptr为NULLstd::lock_guard<std::mutex> lk(mut);
         if(data_queue.empty())
             return std::shared_ptr<T>();
         std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
         data_queue.pop();
         return res;
    }
    void wait_and_pop(T& value) { //若队非空，通过value返回队首元素并删除，函数返回true;若队为空，则通过condition_variable等待有元素入队后再获取闭并删除队首元素
         std::unique_lock<std::mutex> lk(mut);
         data_cond.wait(lk,[this]{return !data_queue.empty();});
         value=data_queue.front();
         data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop() { //和前面一样，只不过通过shared_ptr返回队首元素
         std::unique_lock<std::mutex> lk(mut);
         data_cond.wait(lk,[this]{return !data_queue.empty();});
         std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
         data_queue.pop();
         return res;
    }
    bool empty() const {
         std::lock_guard<std::mutex> lk(mut);
         return data_queue.empty();
    }
};

#endif