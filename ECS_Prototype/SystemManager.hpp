#pragma once
#include <unordered_set>
#include <map>
#include <typeindex>
#include <set>
#include <algorithm>
#include "ISystem.hpp"
#include "Archetype.hpp"
#include "ComponentManager.hpp" 
#include <vector>
#include <thread>
#include <typeindex>
#include "ISystem.hpp"

class SystemScheduler {
private:
    std::vector<std::thread> writingSystems;
    std::vector<std::thread> readingSystems;
    unsigned int sysCount;

public:
    SystemScheduler() : sysCount(0) {}

    void schedule(ISystem & system)
    {
        if ( system.isThreadSafe() ) {
            readingSystems.emplace_back([&system]() {
                system.execute();
                                        });
        } else {
            writingSystems.emplace_back([&system]() {
                system.execute();
                                        });
        }
    }

    void waitForCompletion()
    {
        for ( auto & thread : writingSystems ) {
            if ( thread.joinable() ) {
                thread.join();
            }
        }

        for ( auto & thread : readingSystems ) {
            if ( thread.joinable() ) {
                thread.join();
            }
        }

        writingSystems.clear();
        readingSystems.clear();
    }
};

struct ComponentLoadComparator {
    bool operator()(const std::pair<std::type_index, size_t> & lhs, const std::pair<std::type_index, size_t> & rhs) const
    {
        return lhs.second < rhs.second;
    }
};

class SystemManager {
private:
    SystemScheduler sysScheduler;
    std::set<ISystem *> systems;
    std::map<std::type_index, size_t, ComponentLoadComparator> allComponentsLoads;

public:
    SystemManager() : allComponentsLoads(), sysScheduler() {}

    void addSystem(ISystem & system)
    {
        if ( systems.find(&system) != systems.end() ) {
            return;
        }
        systems.insert(&system);
    }

    void calculateComponentLoad()
    {
        allComponentsLoads.clear();
        for ( auto * sys : systems ) {
            for ( auto idx : sys->getDependencies()->componentIndicies() ) {
                auto cLoad = ComponentManager::getSizeByTypeIndex(idx);
                if ( allComponentsLoads.find(idx) != allComponentsLoads.end() ) {
                    allComponentsLoads[idx] += cLoad;
                } else {
                    allComponentsLoads[idx] = cLoad;
                }
            }
        }
    }

    void delegateWork()
    {
        std::vector<ISystem *> disjointReadSystems;
        std::vector<ISystem *> disjointWriteSystems;
        std::vector<ISystem *> intersectingWriteSystems;

        for ( auto * current : systems ) {
            bool addedToSet = false;

            if ( current->isThreadSafe() ) {
                disjointReadSystems.push_back(current);
            } else {
                for ( auto * other : disjointWriteSystems ) {
                    auto currIdxs = current->getDependencies()->componentIndicies();
                    auto otherIdxs = other->getDependencies()->componentIndicies();

                    if ( !std::none_of(currIdxs.begin(), currIdxs.end(), [&otherIdxs](const std::type_index & idx) {
                        return otherIdxs.find(idx) != otherIdxs.end();
                                       }) ) {
                        intersectingWriteSystems.push_back(current);
                        addedToSet = true;
                        break;
                    }
                }

                if ( !addedToSet ) {
                    disjointWriteSystems.push_back(current);
                }
            }
        }

        // Execute disjoint write systems in parallel
        for ( auto * system : disjointWriteSystems ) {
            sysScheduler.schedule(*system);
        }

        sysScheduler.waitForCompletion();

        // Execute intersecting write systems sequentially
        for ( auto * system : intersectingWriteSystems ) {
            sysScheduler.schedule(*system);
        }

        sysScheduler.waitForCompletion();

        // Execute read systems in parallel
        for ( auto * system : disjointReadSystems ) {
            sysScheduler.schedule(*system);
        }

        sysScheduler.waitForCompletion();
    }

    template <typename System>
    void removeSystem()
    {
        auto it = std::find_if(systems.begin(), systems.end(), [](ISystem * sys) {
            return dynamic_cast<System *>(sys) != nullptr;
                               });

        if ( it != systems.end() ) {
            systems.erase(it);
        }
    }

    template <typename Component>
    std::unordered_set<ISystem *> registeredSystems() const
    {
        std::unordered_set<ISystem *> result;
        for ( auto * system : systems ) {
            if ( system->getDependencies()->componentIndicies().count(typeid(Component)) ) {
                result.insert(system);
            }
        }
        return result;
    }
};
