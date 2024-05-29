#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_set>

class ProfileNode {
public:
    std::string name;
    std::chrono::microseconds elapsed_time;
    std::vector<ProfileNode> children;
    ProfileNode* parent; // Pointer to the parent node

    ProfileNode(const std::string& name, ProfileNode* parent) : name(name), elapsed_time(0), parent(parent) {}

    // Method to add a child node
    void addChild(const ProfileNode& child) {
        children.push_back(child);
    }
};

#define PROFILER_MAX_RECURSION_LIMIT 1000

class Profiler {
private:
    static Profiler* _instance;

    ProfileNode root;
    ProfileNode* current_node;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::unordered_set<std::string> active_func_table;
    int max_recursion_limit;
    int current_recursion_depth;

    void print(const ProfileNode& node, int depth) {
        std::string indent(depth, ' ');
        std::cout << indent << node.name << ": " << node.elapsed_time.count() << " microseconds" << std::endl;
        for (const auto& child : node.children) {
            print(child, depth + 2);
        }
    }

public:
    Profiler() 
        : root{ ProfileNode("Root", nullptr) }, current_node{ &root }, max_recursion_limit{ PROFILER_MAX_RECURSION_LIMIT }, current_recursion_depth{0} 
    {  }

    ~Profiler() 
    { _instance = nullptr; }

    static void start(const std::string& name) {
        if (_instance == nullptr)
        {
            _instance = new Profiler();
        }

        if (_instance->active_func_table.find(name) != _instance->active_func_table.end()) {
            return;
        }
        _instance->active_func_table.insert(name);
        ProfileNode node(name, _instance->current_node);
        _instance->current_node->children.push_back(node);
        _instance->current_node = &(_instance->current_node->children.back());
        _instance->start_time = std::chrono::high_resolution_clock::now();
        ++_instance->current_recursion_depth;
    }

    static void stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        _instance->current_node->elapsed_time += std::chrono::duration_cast<std::chrono::microseconds>(end_time - _instance->start_time);
        _instance->current_node = _instance->current_node->parent;
        _instance->active_func_table.erase(_instance->current_node->name);
        --_instance->current_recursion_depth;
    }

    static void SetRecursionLimit(int max_recursion) {
        _instance->max_recursion_limit = max_recursion;
    }

    static void printResults() {
        std::cout << "Profiling Results:" << std::endl;
        _instance->print(_instance->root, 0);

        if (_instance != nullptr)
            delete _instance;
    }
};

// Operator overrides for starting and stopping the profiler
class ProfilerStartStop {
public:
    ProfilerStartStop(const std::string& name) {
        Profiler::start(name);
    }

    ~ProfilerStartStop() {
        Profiler::stop();
    }
};

#define fn_profiler(name) ProfilerStartStop profilerScope##__LINE__(name)

