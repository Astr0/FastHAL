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
        using index_t = brigand::number_type<VMaxTasks>;
        static constexpr auto capacity = index_t{VMaxTasks};
        //static constexpr auto debug = uart_sync_tx<0>{};

        static auto now() { return timer_t::now(); }

        struct task_due{
            time_t due;
            task_t task;
        };

        volatile bool _changed;
        volatile index_t _size;
        task_due _tasks[capacity];
        //time_t _due[capacity];

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
            // time to next task from start
            auto wait = next_timeout(last);
            //print(debug, "next:");
            //println(debug, wait);

            // wait for time
            time_t elapsed;
            while (((elapsed = now() - last) < wait) && !_changed);

            return elapsed;
        }

        void execute(time_t last, time_t elapsed){
            // index_t i;
            // {
            //     auto lock = no_irq{};
            //     i = _size;
            // }
            auto i = &_tasks[_size];
            task_t task;
            while (i-- != _tasks){
                {
                    auto lock = no_irq{};
                    if (i->due - last > elapsed)
                        continue;
                    task = i->task;
                    *i = _tasks[--_size];
                    //remove_at(i);                                        
                }
                //println(debug, "exec");
                task();
            }
        }

        // void remove_at(index_t i){
        //     auto sz = --_size;
        //     //if (i < sz){
        //         _tasks[i] = _tasks[sz];
        //         _due[i] = _due[sz];
        //     //}
        // }
    public:
        bool setTimeout(time_t timeout, task_t task){
            timeout += timer_t::now();
            auto lock = no_irq{};
            if (_size == capacity)
                return false;
            _tasks[_size++] = task_due { timeout, task };
            // _tasks[_size] = task;
            // _due[_size++] = timeout;
            _changed = true;
            //println(debug, "add");
            return true;
        }

        bool clearTimeout(task_t task){
            auto lock = no_irq{};
            auto i = &_tasks[_size];
            while (i-- != _tasks)
                if (i->task == task){
                    *i = _tasks[--_size];
                    //remove_at(i);
                    _changed = true;
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
                // ok, probably elapsed more than time to next due, lets do some tasks!
                // add if (_changed) continue; to speed up things if changed
                execute(last, elapsed);
                
                last += elapsed;
            }
        }
    };
}

#endif