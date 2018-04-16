#ifndef FH_KERNEL_H_
#define FH_KERNEL_H_

#include "../hal/time.hpp"
#include "../hal/interrupts.hpp"
#include "../utils/list.hpp"

namespace fasthal{
    namespace details{
        template<class TTimer = details::global_time_impl<>>
        struct sys_kernel_timer{
            static auto now() { return TTimer::ms(); }
        };

        struct sys_kernal_default_executor{
            using task_t = void(*)();
            static inline void execute(task_t task){ task(); }
        };
    }

    template<std::size_t VMaxTasks, 
        class TExecutor =  details::sys_kernal_default_executor,
        class TTimer = details::sys_kernel_timer<>>
    struct sys_kernel{
        using timer_t = TTimer;
        using time_t = decltype(timer_t::now());
        using index_t = brigand::number_type<VMaxTasks>;
        using task_t = typename TExecutor::task_t;
        static constexpr auto capacity = index_t{VMaxTasks};
        //static constexpr auto debug = uart_sync_tx<0>{};

        static auto now() { return timer_t::now(); }

        struct task_due{
            time_t due;
            task_t task;

            constexpr task_due(time_t _due, task_t _task): due(_due), task(_task){}
            constexpr task_due(){}
            constexpr task_due(const task_due& other): due(other.due), task(other.task) {}
        };

        //volatile bool _changed;
        volatile index_t _size;
        task_due _tasks[capacity];
        //time_t _due[capacity];

        inline void remove_at(task_due* i){
            *i = _tasks[--_size];
        }


        time_t next_timeout(time_t last){
            time_t result = ~time_t{};

            auto lock = no_irq{};
            auto i = &_tasks[_size];
            while (i-- != _tasks)
            {
                auto to = i->due - last;
                if (to < result)
                    result = to;
            }
            // this will return max timeout for empty tasks!, but we'll wait for change anyway
            return result;
        }        

        time_t wait_next_timeout_or_change(time_t last){
            // don't read atomic - if something changes - it changes сause of volatile
            auto sz = _size;
            // time to next task from start
            auto wait = next_timeout(last);
            //print(debug, "next:");
            //println(debug, wait);

            // wait for time
            time_t elapsed;
            while (((elapsed = now() - last) < wait) && (sz == _size));

            return elapsed;
        }
        
        void execute(time_t last, time_t elapsed){
            auto i = &_tasks[atomic_read(_size)];
            task_t task;
            while (i-- != _tasks){
                {
                    auto lock = no_irq{};
                    if (i->due - last > elapsed)
                        continue;
                    task = i->task;
                    // can't cheat here - no global lock, execute may add something
                    remove_at(i);                                        
                }
                //println(debug, "exec");
                TExecutor::execute(task);
            }
        }
    public:
        bool setTimeout(time_t timeout, task_t task){
            timeout += timer_t::now();
            auto lock = no_irq{};
            auto sz = _size;
            if (sz == capacity)
                return false;
            _tasks[sz] = task_due { timeout, task };
            _size = sz + 1;
            // _changed = true;
            //println(debug, "add");
            return true;
        }

        bool clearTimeout(task_t task){
            auto lock = no_irq{};
            auto i = &_tasks[_size];
            while (i-- != _tasks)
                if (i->task == task){
                    remove_at(i);
                    // _changed = true;
                    return true;
                }
            return false;
        }

        void run(){
            time_t last = 0;                        
            while (true){
                // reset changed since we'll find next due
                // _changed = false;
                auto elapsed = wait_next_timeout_or_change(last);
                // ok, probably elapsed more than time to next due, lets do some tasks!
                // add if (_changed) continue; to speed up things if changed
                execute(last, elapsed);
                
                last += elapsed;
            }
        }
    };
}

#endif