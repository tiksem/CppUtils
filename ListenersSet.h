//
// Created by Semyon Tikhonenko on 5/4/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_LISTENERSSET_H
#define VOCALTRAINER_LISTENERSSET_H

#include <functional>
#include <map>
#include <numeric>
#include "assert.h"
#include <mutex>
#include "Maps.h"

#ifndef NDEBUG
#include <thread>
#include <type_traits>

#define CPPUTILS_LISTENERS_SET_DEBUG_INIT if (threadId == std::thread::id()) { \
    threadId = std::this_thread::get_id(); \
} \
if (std::is_same<Mutex, DummyMutex>::value) \
    assert(threadId == std::this_thread::get_id() && "Listeners were added or/and executed or/and removed from different threads");
#else
#define CPPUTILS_LISTENERS_SET_DEBUG_INIT
#endif

namespace CppUtils {
    enum ListenerAction {
        DONT_DELETE_LISTENER,
        DELETE_LISTENER
    };

    struct DummyMutex {
        inline void lock() {}
        inline void unlock() {}
    };

    template<typename Mutex, typename... Args>
    class BaseListenersSet {
    public:
        typedef std::function<ListenerAction(Args...)> Listener;
    private:
        typedef std::map<int, Listener> Map;
        Mutex mutex;
        Map listeners;
        int nextKey = 1;
#ifndef NDEBUG
        mutable std::thread::id threadId;
#endif
    public:
        int addListenerWithAction(const Listener &func) {
            CPPUTILS_LISTENERS_SET_DEBUG_INIT

            int key;
            {
                std::lock_guard<Mutex> _(mutex);
                key = nextKey++;
                listeners[key] = func;
            }

            return key;
        }

        template <typename Function>
        int addListener(Function func) {
            return addListenerWithAction(Listener([=] (Args... args) -> ListenerAction {
                func(args...);
                return DONT_DELETE_LISTENER;
            }));
        }

        template <typename Function>
        int addListener(Function* func) {
            return addListenerWithAction(Listener([=] (Args... args) -> ListenerAction {
                (*func)(args...);
                return DONT_DELETE_LISTENER;
            }));
        }

        template <typename Function>
        int addOneShotListener(Function func) {
            return addListenerWithAction(Listener([=] (Args... args) -> ListenerAction {
                func(args...);
                return DELETE_LISTENER;
            }));
        }

        template <typename Function>
        int addOneShotListener(Function* func) {
            return addListenerWithAction(Listener([=] (Args... args) -> ListenerAction {
                (*func)(args...);
                return DELETE_LISTENER;
            }));
        }

        bool removeListener(int key) {
            CPPUTILS_LISTENERS_SET_DEBUG_INIT
            std::lock_guard<Mutex> _(mutex);
            return (bool)listeners.erase(key);
        }

        void executeAll(Args... args) {
            CPPUTILS_LISTENERS_SET_DEBUG_INIT

            if (std::is_same<Mutex, DummyMutex>::value) {
                auto end = listeners.end();
                for (auto iter = listeners.begin(); iter != end;) {
                    if (iter->second(args...) == DELETE_LISTENER) {
                        iter = listeners.erase(iter);
                    } else {
                        ++iter;
                    }
                }
            } else {
                std::vector<std::pair<int, Listener>> temp;
                {
                    std::lock_guard<Mutex> _(mutex);
                    temp.assign(listeners.begin(), listeners.end());
                };

                std::vector<int> scheduledForDelete;
                for (const auto& pair : temp) {
                    if (pair.second(args...) == DELETE_LISTENER) {
                        scheduledForDelete.push_back(pair.first);
                    }
                }

                std::lock_guard<Mutex> _(mutex);
                for (int key : scheduledForDelete) {
                    listeners.erase(key);
                }
            }
        }

        void operator()(Args... args) {
            executeAll(args...);
        }

        bool hasListenersToExecute() const {
            CPPUTILS_LISTENERS_SET_DEBUG_INIT
            std::lock_guard<Mutex> _(mutex);
            return !listeners.empty();
        }
    };

    template <typename... Args>
    using ListenersSet = BaseListenersSet<DummyMutex, Args...>;

    template <typename... Args>
    using SynchronizedListenersSet = BaseListenersSet<std::mutex, Args...>;
}

#define CPPUTILS_DEFINE_LISNERS_SET()


#endif //VOCALTRAINER_LISTENERSSET_H
