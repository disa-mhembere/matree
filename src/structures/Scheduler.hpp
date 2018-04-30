/*
 * Copyright 2017 Neurodata (https://neurodata.io)
 * Written by Disa Mhembere (disa@cs.jhu.edu)
 *
 * This file is part of Monya.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MONYA_SCHEDULER_HPP__
#define MONYA_SCHEDULER_HPP__

#include "../common/types.hpp"

#include <vector>
#include <unordered_map>
#include <cmath>
#include <iostream>

#include <pthread.h>

namespace monya { namespace container {

    class NodeView;

    class Scheduler {

        private:
            // Maximum depth of trees permitted
            depth_t max_levels;
            // Depth of the tree being scheduled
            // FIXME: Modification lock reading ..
            std::vector<depth_t> current_level;
            unsigned fanout;
            // Map is: level, nodes in level
            typedef std::unordered_map<unsigned, std::vector<NodeView*> > ln_t;
            ln_t nodes;
            pthread_mutex_t mutex;
            pthread_mutexattr_t mutex_attr;

        public:
            Scheduler(unsigned fanout=2, tree_t ntree=1,
                    depth_t max_depth=MAX_DEPTH);
            // TODO: Remove default value for tree_id
            // Have all the nodes in this level been placed in the queue
            bool is_full(unsigned level, tree_t tree_id=0) {
                // FIXME: Not true for trees > 1
                return std::pow(fanout, level) == nodes[level].size();
            }

            bool is_complete(unsigned level);

            /**
              \brief Add a node to the schedulers list given a particular
              tree id
              */
            void schedule(NodeView* node, const tree_t tree_id=0);

            /**
              \brief Handoff nodes to threads
              */
            void run_level(const depth_t level, const tree_t tree_id);

            const depth_t get_max_depth() const { return max_levels; }

            ~Scheduler();
    };
} } // End namespace monya::container
#endif
