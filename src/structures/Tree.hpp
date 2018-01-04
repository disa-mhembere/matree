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

#ifndef TREE_HPP__
#define TREE_HPP__

#include <memory>
#include "../common/types.hpp"
#include "../common/exception.hpp"
#include "BinaryNode.hpp"

namespace monya { namespace container {

template <typename T>
class Applicator {
    public:
        virtual void call(T*) = 0;
        virtual ~Applicator() { };
};

template <typename T>
class Printer: public Applicator <T> {
    public:
        void call(T* node) override {
            node->print();
        }
};

template <typename T>
class Destroyer: public Applicator <T> {
    public:
        void call(T* node) override {
            delete(node);
        }
};

template <typename T>
class Tree {

    private:
        short numa_id;
        NodeView<T>* root;

        Tree() {
            this->root = NULL;
        }

        Tree(NodeView<T>* root) {
            this->root = root;
        }

    public:
        typedef std::shared_ptr<Tree<T> > ptr;

        static ptr create(NodeView<T>* root=NULL) {
            if (NULL == root)
                return ptr(new Tree<T>());
            else
                return ptr(new Tree<T>(root));
        }

        void materialize_node(const node_id_t id) {
            // TODO
        }

        void virtualize() {
            // TODO
        }

        // Tester
        void echo() {
            if (root) {
                Applicator<BinaryNode<T> >* applicator = new Printer<BinaryNode<T> >();
                apply(BinaryNode<T>::cast2(this->root), applicator, PREORDER);
                delete(applicator);
            }
        }

        NodeView<T>* get_root() {
            return this->root;
        }

        void set_root(NodeView<T>* root) {
            this->root = root;
        }

        void insert(BinaryNode<T>* node, BinaryNode<T>* root) {
            if (!root) {
                root = node;
            } else {

                BinaryNode<T>* prev = NULL;
                BinaryNode<T>* curr = root;

                while (curr) {
                    prev = curr;
                    if (*curr < *node) {
                        curr = BinaryNode<T>::cast2(curr->right());
                    } else {
                        curr = BinaryNode<T>::cast2(curr->left());
                    }
                }

                if (*prev < *node) {
                    prev->right(node);
                } else {
                    prev->left(node);
                }
            }
        }

        NodeView<T>* find(T val) {
            if (NULL == root)
                return NULL; // Found nothing

            BinaryNode<T>* curr = BinaryNode<T>::cast2(root);
            BinaryNode<T>* lookup = new BinaryNode<T>(val);

            while (curr) {
                if (*lookup == *curr) // Users must define the == operator
                    return curr;
                if (*lookup > *curr)
                    curr = BinaryNode<T>::cast2(curr->right());
                else
                    curr = BinaryNode<T>::cast2(curr->left());
            }

            delete(lookup);
            return NULL; // Searched and found nothing!
        }

        // TODO: Optimize
        // TODO: Remove recursion
        // This can be used to perform any method for the tree
        void apply(BinaryNode<T>* node, Applicator<BinaryNode<T> >* applicator,
                order_t traversal) {
            switch (traversal) {
                case POSTORDER:
                    if (node->left()) // Checks NULL, but TODO: should also check if on disk
                        apply(BinaryNode<T>::cast2(node->left()), applicator, traversal);
                    if (node->right())
                        apply(BinaryNode<T>::cast2(node->right()), applicator, traversal);

                    applicator->call(node);
                    break;
                case INORDER:
                    if (node->left())
                        apply(BinaryNode<T>::cast2(node->left()), applicator, traversal);

                    applicator->call(node);

                    if (node->right())
                        apply(BinaryNode<T>::cast2(node->right()), applicator, traversal);
                    break;
                case PREORDER:
                    applicator->call(node);

                    if (node->left())
                        apply(BinaryNode<T>::cast2(node->left()), applicator, traversal);
                    if (node->right())
                        apply(BinaryNode<T>::cast2(node->right()), applicator, traversal);
                    break;
                case LEVELORDER:
                    throw monya::not_implemented_exception();
                    break;
                default:
                    throw monya::parameter_exception(std::to_string(traversal));
            }
        }

        ~Tree() {
            if (root) {
                Applicator<BinaryNode<T> >* applicator = new Destroyer<BinaryNode<T> >();
                apply(BinaryNode<T>::cast2(this->root), applicator, POSTORDER);
                delete(applicator);
            }
        }
};

} } // End monya::container

#endif