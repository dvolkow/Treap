//============================================================================
// Name            : treap.hpp
// Author          : Wolkow Daniel
// Description     : This hpp implements treap = tree + heap, Binary Search Tree. 
//============================================================================

#ifndef TREAP_HPP
#define TREAP_HPP 1

#include <random>
#include <cassert>
#include <ctime>
#include <memory>

#include <iostream>


namespace bst 
{
    class Randomizer
    {
        std::mt19937 gen;  // -- ГПСЧ

    public:
        Randomizer() noexcept
        {
            gen.seed(time(0));
        }

        Randomizer(time_t seed) noexcept
        {
            gen.seed(seed);
        }

        size_t operator()() noexcept
        {
            return gen();
        }

        ~Randomizer() {}
    };

//-----------------------------------------------------------------------------
   Randomizer rand_;
//-----------------------------------------------------------------------------

    template <typename T> 
    class treap
    {
        struct node;
        using pnode = std::shared_ptr<node>;

        //---------------------------------------------------------------------
        // idea: using this
        //std::vector<node> tree_;

        pnode root_; // -- корень
        T key_; // -- ключ, который был положен в корень первым
        mutable bool success_; // -- успешность последней операции над деревом

    public:
//        typedef treapIterator<T> iterator;
//        typedef treapIterator<const T> const_iterator;

        treap() noexcept
            : root_(nullptr), key_(T()), success_(false) 
        { rand_ = Randomizer(); }

        treap(const treap & other) 
            : root_(other.root_), key_(other.key_), success_(other.success_) 
        { rand_ = Randomizer(); }

        treap(std::initializer_list<T> init) 
            : root_(nullptr), key_(), success_(false) 
        { 
            rand_ = Randomizer(); 
            for (auto i : init)
                this->insert(i);
        }

 //       iterator begin();
//        iterator end();
 //       const_iterator begin();
//        const_iterator end();
        /**
         * Поиск в дереве по ключу
         * @param key -- значение ключа
         * @return true, если элемент есть в дереве и не отмечен как удалённый,
         * false в противном случае
         */
        bool find(const T key) const noexcept 
        {    
            return find_(root_, key); 
        }

        /**
         * Поиск в дереве по ключу
         * @param key -- значение ключа
         * @return true, если элемент есть в дереве, false в противном случае
         */
        bool was(const T key) const noexcept 
        { 
            return was_(root_, key); 
        }

        /**
         * Точная верхняя грань по ключу
         * @param key -- значение ключа
         * @return минимальный ключ t из дерева такой, что t > key
         */
        T upper_bound(const T key) const noexcept 
        {    
            return upper_bound_(root_, key); 
        }

        /**
         * Максимальный элемент, больший ключа
         * @param key -- значение ключа
         * @return ключ из дерева
         */
        T previous(const T key) const noexcept 
        { 
            return previous_(root_, key); 
        }

        /**
         * Вставка по ключу в дерево
         * @param key -- значение ключа
         */
        void insert(const T key)
        {
            if (revive_(root_, key))
            {
                success_ = false;
                return;
            }

            if (!find_(root_, key))
            {
                insert_(root_, std::make_shared<node>(node(key)));
            }

            success_ = false;
        }

        /**
         * Перемещает кусок массива от левой до правой границы в начало.
         * @param left_bound -- левая граница (индексация с единицы!)
         * @param right_bound -- правая граница
         */
        void replace_in_head(const size_t left_bound, const size_t right_bound)
        {
            /**
             * Режем раз:
             */
            std::pair<pnode, pnode> left_middle_cut = _split_(root_, left_bound - 1);

            /**
             * Режем два:
             */
            std::pair<pnode, pnode> middle_right_cut = _split_(left_middle_cut.second,
                                                               right_bound - left_bound + 1);

            /**
             *Клеим крайние деревья:
             */
            pnode tmp = nullptr;
            merge(tmp, left_middle_cut.first, middle_right_cut.second);

            /**
             * Клеим вырезанное и склеенное:
             */
            pnode new_root = nullptr;
            merge(new_root, middle_right_cut.first, tmp);
            root_ = new_root;
        }

        /**
         * Hard-удаление (не игнорирует мягко удалённые, удаляет НЕЗАВИСИМО
         * от того, был ли данный элемент мягко удален. Соответственно,
         * вызывает специальный вариант функции find)
         * @param key -- значение ключа
         */
        void erase(const T key)
        {
            if (find_(root_, key)) 
                remove_(root_, key);

            success_ = false;
        }

        /**
         * Soft-удаление. Удаляемые элементы просто помечаются как удаленные,
         * физически оставаясь на своем месте и участвуя во всех операциях,
         * кроме поиска, удаления, точной верхней грани и подобного.
         * @param key -- значение ключа
         * @return true, если key присутствовал неудаленным в дереве до вызова
         */
        bool remove(const T key) 
        {
            if (soft_remove_(root_, key)) 
            {
                return true;
            }

            return false;
        }

        /**
         * Обратная индексация в обычном и неявном дереве
         * @param k -- номер максимума в дереве
         * @return T key : k-й максимум в дереве
         */
        T k_max(const size_t k) const 
        { 
            return k_max_(root_, k); 
        }

        /**
         * Размер дерева
         * @return количество элементов в дереве
         */
        size_t size() const 
        {
            return root_ ? root_->count_ : 0;
        }

        /**
         * Обработка ошибок:
         * @return true, если последняя операция завершилась успешно
         */
        bool get_success() const 
        { 
            return success_; 
        }

        /**
         * Печать дерева
         */
        void print() const { print_(root_); }

    private:

        /**
         * Даёт размер поддеревьев для данного корня: 
         */
        size_t get_count_(const pnode root) const 
        {
            return root ? root->count_ : 0;
        }

        /**
         * Обновление размеров поддеревьев после каждой mutable операци
         */
        void update_count_(pnode & root) 
        {
            if (root) 
                root->count_ = 1 + get_count_(root->l) + get_count_(root->r);
        }

        /**
         * Склеивает два дерева в одно
         * @param root -- корень нового дерева
         * @param l -- левое дерево на склейку
         * @param r -- правое дерево на склейку
         */
        void merge(pnode & root, pnode l, pnode r) 
        {
            if (!l || !r)
                root = l ? l : r;
            else if (l->priority_ > r->priority_)
                merge(l->r, l->r, r), root = l;
            else
                merge(r->l, l, r->l), root = r;

            update_count_(root);
        }

        /**
         * Разделяет дерево надвое по ключу
         * @param root -- корень исходного дерева
         * @param key -- значение ключа
         * @param l -- левое дерево
         * @param r -- правое дерево
         */
        void split(pnode root, const T key, pnode & l, pnode & r) 
        {
            if (!root)
                l = r = nullptr;
            else if (key < root->key_) 
            {
                split(root->l, key, l, root->l); 
                r = root;
            }
            else 
            {
                split(root->r, key, root->r, r);  
                l = root;
            }

            update_count_(root);
        }

        /**
         * Вставка в дерево
         * @param root -- текущий корень
         * @param it -- узел для вставки
         */
        void insert_(pnode & root, pnode it) 
        {
            /* Если пустое! */
            if (size() == 0)
            {
                root_ = it;
                update_count_(root_);
                this->key_ = it->key_;
                return;
            }

            /* Если только один элемент! */
            if (size() == 1)
            {
                if (root_->key_ > it->key_)
                    root_->l = it;
                else if (root_->key_ < it->key_) 
                    root_->r = it;
                else if (root_->key_ == it->key_)
                    return;

                update_count_(root);
                update_count_(it);
                return;
            }

            if (!root)
                root = it;
            else if (it->priority_ > root->priority_)
                /* Останавливаемся на первом элементе, в котором значение
                 * приоритета оказалось меньше: 
                 */
                split(root, it->key_, it->l, it->r),  root = it;
            else if (it->key_ < root->key_)
                /* Спускаемся по дереву, как в обычном бинпоиске */    
                insert_(root->l, it);
            else if (it->key_ > root->key_)
                insert_(root->r, it);

            update_count_(root);
        }

        /**
         * Вставка в пустое дерево
         * @param root -- корень дерева
         * @param it -- узел для вставки
         */
        void insert_in_empty_(pnode it)
        {
            root_ = it;
            update_count_(root_);
            this->key_ = it->key_;
            return;
        }

        /**
         * Размер поддерева для узла
         * @param root -- узел
         * @return размер поддерева
         */
        size_t sOf_(const pnode & root) const
        {    
            return root != nullptr ? root->count_ : 0;
        } 

        /**
         * k-й максимум
         * @param root -- корень текущего поддерева
         * @param k -- текущее значение индекса
         * @return ключ k-го максимального узла
         */
        T k_max_(const pnode & root, size_t k) const
        {
            pnode current = root;
            while (current)
            {
                size_t sR = sOf_(current->r);
                if (sR == k) 
                {
                     success_ = true; 
                     return current->key_; 
                }

                current = sR > k ? current->r : current->l;

                if (k > sR)
                    k -= sR + 1;
            }

            success_ = false;
            return T();
        }


        /**
         * Удаление элемента. Спускаемся по дереву бинпоиском, найдя
         * элемент, вызываем merge сыновей и записываем результат на
         * место найденного элемента:
         * @param root -- текущий узел
         * @param key -- значение ключа
         */
        void remove_(pnode & root, const T key)
        {
            if (root->key_ == key)
                merge(root, root->l, root->r);
            else
                remove_(key < root->key_ ? root->l : root->r, key);

            update_count_(root);
        }

        /**
         * Мягкое удаление
         * @param root -- текущий узел
         * @param key -- значение ключа
         * @return true, если найден и удален
         */
        bool soft_remove_(pnode & root, const T key)
        {
            if (!root) return false;

            if (root->key_ == key) 
                return root->testDel() ? false : true ; 

            return root->key_ < key ? soft_remove_(root->r, key) : soft_remove_(root->l, key); 
        }

        /**
         * Бинпоиск в дереве
         * @param root -- текущий узел
         * @param key -- значение ключа
         * @return true, если найден
         */
        bool find_(const pnode & root, const T key) const 
        {
            if (!root) return false;

            if (root->key_ == key && !root->deleted_) 
                return true;

            if (root->key_ > key) 
                return find_(root->l, key);

            if (root->key_ < key) 
                return find_(root->r, key);

            return false;
        }

        /**
         * Вывод дерева в inorder-обходе:
         * @param root -- текущий узел
         */
        void print_(const pnode root) const
        {
            if (!root) return;

            print_(root->l);
            if (!root->deleted_)
                std::cout << "(" << root->key_ <<  ", " << root->count_ << /*  ", prior= " <<
                            root->prior<<*/ ") ";
            else
                std::cout << "(D-" << root->key_ << /* ", " << root->count  <<  ", prior= " <<
                            root->prior<<*/ ") ";

            print_(root->r);
        }

        /**
         * Следующий в дереве по ключу
         * @param root
         * @param key
         * @return
         */
        T upper_bound_(const pnode & root, const T key) const
        {
            if (!root_ || !root) 
            {
                 success_ = false; 
                return key;
            }

            pnode current = root;
            pnode result = nullptr;

            while (current) 
            {
                if (current->key_ > key)
                    result = current, current = current->l;
                else 
                    current = current->r;
            }

            if (!result) 
            {
                success_ = false; 
                return key;
            } 
            else if (result->deleted_) 
            {
                success_ = false; 
                return upper_bound_(root_, result->key_);
            } 
            else 
            {
                success_ = true; 
                return result->key_;
            } 
        }

        /**
         * Предыдущий в дереве по ключу
         * @param root
         * @param key
         * @return
         */
        T previous_(const pnode & root, const T key) const 
        {
            success_ = false; 
            if (!root_ || !root) 
            {
                return key;
            }

            pnode current = root_;
            pnode result = nullptr;

            while (current) 
            {
                if (current->key_ < key)
                    result = current, current = current->r;
                else 
                    current = current->l;
            }

            if (!result) 
            {
                return key;
            } 
            else if (result->deleted_) 
            {
                return previous_(root_, result->key_);
            } 
            else 
            {
                success_ = true; 
                return result->key_;
            }
        }

        /**
         * Восстановление ранее удаленного при вставке
         * @param root -- текущий узел
         * @param key -- значение ключа
         * @return true, если элемент найден и оживлен, false во всех остальных
         */
        bool revive_(pnode & root, const T key)
        {
            if (!root) 
                return false;

            if (root->key_ == key)
            {
                if (root->deleted_)
                    root->invertDel();
                return true;
            } 
            else if (root->key_ > key) 
                return revive_(root->l, key);
            else if (root->key_ < key)
                return revive_(root->r, key);

            return false;    
        }

        /**
         * Вариант find_, игнорирующий удалённые soft'ом ключи
         * @param root -- текущий узел
         * @param key -- значение ключа
         * @return true, если элемент ранее вставляли в дерево
         * и к нему не была применена операция erase
         */
        bool was_(const pnode & root, const T key) const
        {
            if (!root) 
                return false;

            if (root->key_ == key) 
                return true;

            if (root->key_ > key) 
                return was_(root->l, key);

            if (root->key_ < key) 
                return was_(root->r, key);

            return false;
        }

        /**
         *--------------------------------------------------------
         *--ДЕКАРТОВО ДЕРЕВО ПО НЕЯВНОМУ КЛЮЧУ-----ОПЕРАЦИИ-------
         *--------изменилась только операция split:---------------
         *--------------------------------------------------------
         * @param root -- текущий корень
         * @param count -- количество отрезаемых вершин
         * @return std::pair<pnode, pnode> -- пара корней новых де
         * ревьев.
         */
        std::pair<pnode, pnode> _split_(pnode root, const size_t count)
        {
            if (!root)
                return std::make_pair(nullptr, nullptr);

            size_t left_size = sOf_(root->l);
            if (left_size >= count)
            {
                std::pair<pnode, pnode> res = _split_(root->l, count);
                root->l = res.second;
                update_count_(root);
                return std::make_pair(res.first, root);
            } 
            else
            {
                std::pair<pnode, pnode> res = _split_(root->r, count - left_size - 1);
                root->r = res.first;
                update_count_(root);
                return std::make_pair(root, res.second);
            }
        }

        /**
         * @param root -- корень дерева
         * @param idx -- индекс массива настоящего состояния
         * @return указатель на k-ю вершину дерева по неявному ключу
         */
        pnode _index_in_rope(pnode & root, size_t idx)
        {
            pnode current = root;

            while (current)
            {
                size_t sL = sOf_(current->l);
                if (sL == idx) 
                {
                    success_ = true;
                    return current;
                }

                current = sL > idx ? current->l : current->r;

                if (idx > sL)
                    idx -= sL + 1;
            }

            success_ = false;
            return nullptr;
        }

        //---------------------------------------------------------------------
        struct node 
        {
            pnode l; // -- левый потомок
            pnode r; // -- правый потомок
            T key_; // -- пользовательский ключ
            size_t priority_; // -- приоритет
            size_t count_; // -- размер поддерева текущей вершины
            bool deleted_; // -- флаг для быстрого удаления (remove)

            node() noexcept
                    : l(nullptr), r(nullptr), key_(0), priority_(rand_()), 
                      count_(0), deleted_(false) 
            {}

            node(const T& key) noexcept
                    : l(nullptr), r(nullptr), key_(key), priority_(rand_()), 
                      count_(1), deleted_(false) 
            {}

            node(const node & another) noexcept
                    : l(another.l), r(another.r), key_(another.key_),
                      priority_(rand_()), count_(another.count_), deleted_(another.deleted_)
            {}

            node(const T& key, const size_t priority) noexcept 
                    : l(nullptr), r(nullptr), key_(key), priority_(priority), 
                      count_(0), deleted_(false) 
            {}

            ~node() {}

            /**
             * Проверяет, удален ли узел. Если не удален, меняет состояние!
             * @return true, если узел уже был помечен как удаленный
             */
            bool testDel() noexcept
            {
                if (!deleted_)
                {
                    invertDel();
                    return false;
                } 
                return true;
            }

            void invertDel() noexcept 
            { 
                deleted_ = !deleted_; 
            }
        }; // -- node 

        template <typename ValueType>
        class treapIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType>
        {
            friend class treap;
        private: 
            treapIterator(ValueType * p);
        public:
            treapIterator(const treapIterator & it);

            bool operator!=(treapIterator const & other) const;
            bool operator==(treapIterator const & other) const; // need for boost foreach
            typename treapIterator::reference operator*() const;
            treapIterator & operator++();
        private:
            ValueType * p;
        };
    };
}

#endif
