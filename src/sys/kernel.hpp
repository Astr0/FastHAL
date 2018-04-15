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
        using due_t = decltype(timer_t::now());
        //static constexpr auto debug = uart_sync_tx<0>{};

        class task_time{            
            due_t _due;
            task_t _task;
        public:
            task_time(){}
            task_time(due_t due, task_t task): _due(due), _task(task) {}

            bool for_task(task_t task) { return _task == task; }

            due_t due_elapsed(due_t time) { return _due - time; }
            task_t task() { return _task; }
        };
  

        static auto now() { return timer_t::now(); }

        list<task_time, VMaxTasks> _tasks;
        volatile bool _changed;

        void tasks_changed() { _changed = true; }

        due_t next_due_elapsed(due_t last){
            while (true){
                // wait for tasks
                while (_tasks.empty()){                    
                }

                {                    
                    auto lock = no_irq{};
                    // someone removed the only task =/
                    auto sz = _tasks.size();
                    if (sz == 0)
                        continue;
                    auto result = _tasks.at(--sz).due_elapsed(last);
                    while (sz--)
                    {
                        auto due = _tasks.at(sz).due_elapsed(last);
                        if (due < result)
                            result = due;
                    }
                    return result;                    
                }
            }
        }        

        due_t wait_next_due_or_change(due_t last){
            // time to next task from start
            auto next_due = next_due_elapsed(last);
            //print(debug, "next:");
            //println(debug, next_due);

            // wait for time
            due_t elapsed;
            while (((elapsed = now() - last) < next_due) && !_changed);

            return elapsed;
        }

        task_t pop_due_task(due_t last, due_t elapsed){
            auto lock = no_irq{};

            auto i = _tasks.size();
            while (i--){
                if (_tasks.at(i).due_elapsed(last) <= elapsed)
                {
                    auto task = _tasks.at(i).task();
                    _tasks.remove_at(i);
                    return task;
                }
            }
            return nullptr;
        }
    public:
        bool setTimeout(due_t timeout, task_t task){
            auto lock = no_irq{};
            if (_tasks.full()){
                //println(debug, "nadd");
                
                return false;
            }
            auto due = timer_t::now() + timeout;
            _tasks.add(task_time{ due, task });
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
            due_t last = 0;                        
            while (true){
                // reset changed since we'll find next due
                _changed = false;
                auto elapsed = wait_next_due_or_change(last);
                // restart if changed?                
                // ok, probably elapsed more than time to next due, lets do some tasks!
                // add if (_changed) continue; to speed up things if changed
                task_t task;
                while ((task = pop_due_task(last, elapsed)) != nullptr){
                    //println(debug, "exec");
                    task();
                }
                last += elapsed;
            }
        }
    };
}

#endif