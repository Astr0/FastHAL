#ifndef FH_KERNEL_H_
#define FH_KERNEL_H_

#include "../hal/time.hpp"
#include "../hal/interrupts.hpp"
#include "../utils/list.hpp"

namespace fasthal{
    using task_t = void(*)();

    namespace details{
        template<class TTimer = details::global_time_impl<>>
        struct sys_kernel_timer{
            static auto now() { return TTimer::ms(); }
        };
    }

    template<std::size_t VMaxTasks, class TTimer = details::sys_kernel_timer<>>
    struct sys_kernel{
        using timer_t = TTimer;
        using time_t = decltype(timer_t::now());
        //static constexpr auto debug = uart_sync_tx<0>{};

        class task_time{            
            time_t _due;
            task_t _task;
        public:
            task_time(){}
            task_time(time_t due, task_t task): _due(due), _task(task) {}

            bool for_task(task_t task) { return _task == task; }

            time_t timeout(time_t time) { return _due - time; }
            task_t task() { return _task; }
        };
  

        static auto now() { return timer_t::now(); }

        volatile bool _changed;
        list<task_time, VMaxTasks> _tasks;

        void tasks_changed() { _changed = true; }

        time_t next_timeout(time_t last){
            time_t result = ~time_t{};

            auto lock = no_irq{};
            auto sz = _tasks.size();
            while (sz--)
            {
                auto to = _tasks.at(sz).timeout(last);
                if (to < result)
                    result = to;
            }
            // this will return max timeout for empty tasks!, but we'll wait for change anyway
            return result;
        }        

        time_t wait_next_timeout_or_change(time_t last){
            // time to next task from start
            auto wait = next_timeout(last);
            //print(debug, "next:");
            //println(debug, next_due);

            // wait for time
            time_t elapsed;
            while (((elapsed = now() - last) < wait) && !_changed);

            return elapsed;
        }

        task_t pop_time_task(time_t last, time_t elapsed){
            auto lock = no_irq{};

            auto i = _tasks.size();
            while (i--){
                if (_tasks.at(i).timeout(last) <= elapsed)
                {
                    auto task = _tasks.at(i).task();
                    _tasks.remove_at(i);
                    return task;
                }
            }
            return nullptr;
        }
    public:
        bool setTimeout(time_t timeout, task_t task){
            timeout += timer_t::now();
            auto lock = no_irq{};
            if (_tasks.full())
                return false;
            _tasks.add(task_time{ timeout, task });
            tasks_changed();
            //println(debug, "add");
            return true;
        }

        bool clearTimeout(task_t task){
            auto lock = no_irq{};
            auto i = _tasks.size();
            while (i--)
                if (_tasks.at(i).for_task(task)){
                    _tasks.remove_at(i);
                    tasks_changed();
                    return true;
                }
            return false;
        }

        void run(){
            time_t last = 0;                        
            while (true){
                // reset changed since we'll find next due
                _changed = false;
                auto elapsed = wait_next_timeout_or_change(last);
                // restart if changed?                
                // ok, probably elapsed more than time to next due, lets do some tasks!
                // add if (_changed) continue; to speed up things if changed
                task_t task;
                while ((task = pop_time_task(last, elapsed)) != nullptr){
                    //println(debug, "exec");
                    task();
                }
                last += elapsed;
            }
        }
    };
}

#endif