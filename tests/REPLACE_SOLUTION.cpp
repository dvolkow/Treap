//============================================================================
// Name       	 : Treap
// Author     	 : Wolkow Daniel
// Version    	 : 1.0
// Description	 : This hpp implements treap = tree + heap, Binary Search Tree. 
// Interface  	 : 
//			  	 :
//============================================================================
#ifndef TREAP_HPP
#define TREAP_HPP 1

#include <algorithm>
#include <random>
#include <cassert>
#include <ctime>

#define FAST_ALLOCATION 1
//#define CONSTRUCTOR_FROM_NODE 1
#define CONTEST 1

#ifdef DEBUG 
	#include <iostream>
#else
	#ifdef CONTEST
		#include <iostream>
	#endif
#endif


namespace bst {

#ifdef FAST_ALLOCATION 
	/**
	  * For fast allocation 
	  */
	const size_t MAX_MEM = 1e8;
	size_t mpos = 0;
	char mem[MAX_MEM];
#endif		

#ifdef STATS 
	size_t counter_allocate = 0;
#endif		

	std::mt19937 gen; 

	template<typename T, class Compare = std::less<T> >
	class node {
	public:	
		node * l;
		node * r;
	private:	
		unsigned int count_;
		int priority_;
		T key_;
		bool deleted_;

	public:	
		node() {
			this->l = NULL;
			this->r = NULL;
			this->count_ = 0;
			this->deleted_ = false;
		}

		node(const T& key) {
			this->key_ = key;
			this->priority_ = (int)gen();
			this->l = NULL;
			this->r = NULL;
			this->count_ = 1;
			this->deleted_ = false;
		}

#ifdef CONSTRUCTOR_FROM_NODE 
		node(const node & another) 
		{
			this->key_ = another.key_;
			this->priority_ = (int)gen();
			this->l = another.l;
			this->r = another.r;
			this->count_ = another.count_;
			this->deleted_ = another.deleted_;
		}
#endif		

		node(const T& key, const int priority) : priority_(priority), 
											l(nullptr), r(nullptr), count_(0), key_(key), deleted_(false) { }

		void up_count() { ++count_;}

		void down_count() 
		{ 
			if(count_ > 0) 
					--count_; 
#ifdef DEBUG 
			else 
				std::cerr << "down_count for count < 1\n";
#endif		
		}

		/**
		 * Размер поддерева текущей вершины в декартовом дереве
		 */
		unsigned int get_count() const { return count_; }
		int get_priority() const { return priority_; }
		const T get_key() const { return key_; }
		void set_count(const unsigned int count) { count_ = count; }

		/**
		 * Return true if node was soft deleted.
		 */
		bool isDeleted() const { return deleted_; }
		void invertDel() { deleted_ = !deleted_; }

		/**
		 * ИЗМЕНЯЕТ СОСТОЯНИЕ deleted_, если оно false!!!!
		 */
		bool testDel() 
		{ 
			//---Если node НЕ удалена:
			if (!isDeleted())
			{
				invertDel();
				return false;
			} else {
				return true;
			}
		}
			
#ifdef FAST_ALLOCATION 
		void * operator new(size_t n) 
		{
			assert((mpos += n) <= MAX_MEM && "Ошибка аллокатора");
#ifdef STATS 
			++counter_allocate;
#endif		
			return (void *)(mem + mpos - n);
		}
	
		void operator delete(void *) noexcept { }
#endif		
	};


	template<typename T> 
	class treap
	{
		typedef node<T>* pnode;
	private:	
		pnode root_;
		T key_;
		size_t size_;
		size_t deleted_count_;
		bool success_;

	public:
		treap() : root_(nullptr), key_((T)0), size_(0), deleted_count_(0),  success_(0) {gen.seed(time(0));};
		
		/**
		 * Поиск элемента (не находит, если было мягкое удаление)
		 */
		bool find(T key) const
		{
			return find_(root_, key);
		} 		

		/**
		 * Был ли элемент когда-либо добавлен в дерево
		 */
		bool was(T key) const
		{
			return was_(root_, key);
		}

#ifdef STATS 
		/**
		 * Количество "удалённых" в дереве
		 */
		size_t deleted_count() { return deleted_count_; }
#endif		

		/**
		 * Минимальный элемент, больший ключа
		 */
		T upper_bound(T key)
		{
			return upper_bound_(root_, key);
		}

		/**
		 * Максимальный элемент, меньший ключа
		 */
		T previous(T key)
		{
			return previous_(root_, key);
		}
		
		/**
		 * Вставка
		 */
		void insert(T key)
		{
			if (revive_(root_, key))
			{
				down_deleted_count_();
				up_size();
				success_ = false;
				return;
			}

			if(!find_(root_, key)) {
				insert_(root_, new node<T>(key));
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
#ifdef DEBUG 
			assert(left_bound <= root_->get_count() && right_bound <= root_->get_count());
#endif		

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
            pnode tmp = NULL;
			merge(tmp, left_middle_cut.first, middle_right_cut.second);

			/**
			 * Клеим вырезанное и склеенное:
			 */
			pnode new_root = NULL;
			merge(new_root, middle_right_cut.first, tmp);
			root_ = new_root;
		}

#ifdef CONTEST
		void contest_output()
		{
			result_(root_);
		}
#endif

		/**
		 * Hard-удаление (не игнорирует мягко удалённые, удаляет НЕЗАВИСИМО
		 * от того, был ли данный элемент мягко удален. Соответственно,
		 * вызывает специальный вариант функции find)
		 */
		void erase(T key)
		{
			if(find_(root_, key)) remove_(root_, key);
			success_ = false;
		}

		/**
		 * Soft-удаление
		 */
		bool remove(T key) 
		{
			if (soft_remove_(root_, key)) 
			{
				up_deleted_count_();
				return true;
			}

			return false;
		}

		T k_max(unsigned int k)
		{
			return k_max_(root_, k);
		}

#ifdef DEBUG 
		/**
		 * Печать дерева
		 */
		void print() 
		{
			print_(root_);
		}
#endif		

		/**
		 * Корректность операций upper_bound 
		 */
		bool get_success() { return success_; }

		/**
		 * Размер дерева O(1)
		 */
		size_t size()
		{
			return root_ ? root_->get_count() : 0;
		}

		/** 
		 * @DEPRECATED
		 *
		 * O(Log(N)) !!!!  Размер дерева
		 */
		size_t size_n()
		{
			size_t tmp = 0;
			size_n_(root_, tmp);
			return tmp;
		}


	private:

		/**
	 	 * Даёт размер поддеревьев для данного корня: 
	 	 */
		unsigned int get_count_(const pnode root) {
			return root ? root->get_count() : 0;
		}

		void size_n_(pnode root, size_t & count)
		{
			if (!root) return;

			size_n_(root->l, count);
			++count;
			size_n_(root->r, count);
		}
		/**
		 * Обновление размеров поддеревьев после каждой mutable операци
		 */
		inline void update_count_(pnode & root) 
		{
			if (root) 
				root->set_count(1 + get_count_(root->l) + get_count_(root->r));
		}

		/**
	 	 * Объединяет два поддерева и возвращает новое курево.
	 	 */
		void merge(pnode & root, pnode l, pnode r) 
		{
			if (!l || !r)
				root = l ? l : r;
			else if (l->get_priority() > r->get_priority())
				merge(l->r, l->r, r), root = l;
			else
				merge(r->l, l, r->l), root = r;

			update_count_(root);
		}

		/**
 	 	 * Разделяет курево root на два поддерева по ключу key:	 
 	 	 */
		void split (pnode root, const T key, pnode & l, pnode & r) 
		{
			if (!root)
				l = r = NULL;
			else if (key < root->get_key()) {
				split(root->l, key, l, root->l); 
				r = root;
			}
			else {
				split(root->r, key, root->r, r);  
				l = root;
			}

			update_count_(root);
		}

		/**
	 	 * Выполняет добавление в курево нового элемента
	 	 */
		void insert_(pnode & root, pnode it) 
		{

			/* Если пустое! */
			if (size_ == 0)
			{
				root_ = it;
				update_count_(root_);
				up_size();
				this->key_ = it->get_key();
				return;
			}

			/* Если только один элемент! */
			if (size_ == 1)
			{
				if (root_->get_key() > it->get_key())
					root_->l = it;
				else if (root_->get_key() < it->get_key()) 
					root_->r = it;
				else if (root_->get_key() == it->get_key())
					return;

				update_count_(root);
				update_count_(it);
				up_size();
				return;
			}

		
			if (!root)																																				
				root = it,
				up_size();
			else if (it->get_priority() > root->get_priority())
				/* Останавливаемся на первом элементе, в котором значение
			 	 * приоритета оказалось меньше: 
			 	 */
				split(root, it->get_key(), it->l, it->r),  root = it, up_size();
			else if (it->get_key() < root->get_key())
				/* Спускаемся по дереву, как в обычном бинпоиске */	
				insert_(root->l, it);
			else if (it->get_key() > root->get_key())
				insert_(root->r, it);

			update_count_(root);
		}

		/**
		 * Insert in empty treap
		 */
		void insert_in_empty_(pnode & root, pnode it)
		{
			root_ = it;
			update_count_(root_);
			up_size();
			this->key_ = it->get_key();
			return;
		}

		size_t sOf_(pnode & root)
		{	
			return root != nullptr ? root->get_count() : 0;
		} 

		T k_max_(pnode & root, unsigned int k)
		{
			pnode current = root;
			while (current)
			{
				size_t sR = sOf_(current->r);
				if (sR == k) {
				 	success_ = true; 
				 	return current->get_key(); 
				}

				current = sR > k ? current->r : current->l;

				if (k > sR)
					k -= sR + 1;
				assert(k>=0);
			}

			success_ = false;
			return T();
		}


		/**
		 * Удаление элемента. Спускаемся по дереву бинпоиском, найдя
	 	 * элемент, вызываем merge сыновей и записываем результат на 
	 	 * место найденного элемента:
	 	 */
		void remove_(pnode & root, const T key) 
		{
			if (root->get_key() == key)
				merge(root, root->l, root->r), down_size();
			else
				remove_(key < root->get_key() ? root->l : root->r, key);
			update_count_(root);
		}

		/**
		 * Мягкое удаление
		 */
		bool soft_remove_(pnode & root, const T key)
		{
			if (!root) return false;

			if (root->get_key() == key) 
			{
				return root->testDel() ? false : true ; 
			}  	
			return root->get_key() < key ? soft_remove_(root->r, key) : soft_remove_(root->l, key); 
		}

		/**
	 	 * Бинпоиск в дереве
	     */
		bool find_(pnode & root, const T key)
		{
			if (!root) return false;

			if (root->get_key() == key && !root->isDeleted()) return true;
			if (root->get_key() > key) return find_(root->l, key);
			if (root->get_key() < key) return find_(root->r, key);

			return false;
		}

#ifdef DEBUG 
		/**
	 	 * Вывод дерева в inorder-обходе:
	 	 */
		void print_(pnode root) {
			if (!root) return;


			print_(root->l);
			if (!root->isDeleted()) 
				std::cout << "(" << root->get_key() <<  ", " << root->get_count() << /*  ", prior= " << 
							root->prior<<*/ ") ";
			else	
				std::cout << "(D-" << root->get_key() << /* ", " << root->count  <<  ", prior= " << 
							root->prior<<*/ ") ";

			print_(root->r);
		}
#endif

		/**
	 	 * Точная верхняя грань по ключу
	  	 */
		T upper_bound_(pnode & root, const T key) {

			if (!root_ || !root) {
			 	success_ = false; 
				return key;
			}

			pnode current = root;
			pnode result = nullptr;

			while (current) 
			{
				if (current->get_key() > key)
				{
					result = current;
					current = current->l;
				} else {
					current = current->r;
				}
			}

			if (!result) {
			 	success_ = false; 
				return key;
			} else if (result->isDeleted()) {
			 	success_ = false; 
				return upper_bound_(root_, result->get_key());
			} else {
			 	success_ = true; 
				return result->get_key();
			} 
		}

		/**
		 * Максимальный элемент, меньший ключа
		 */
		T previous_(pnode & root, const T key) {
			if (!root_ || !root) {
		 		success_ = false; 
				return key;
			}

			pnode current = root_;
			pnode result = nullptr;

			while (current) 
			{
				if (current->get_key() < key)
				{
					result = current;
					current = current->r;
				} else {
					current = current->l;
				}
			}

			if (!result) {
			 	success_ = false; 
				return key;
			} else if (result->isDeleted()) {
			 	success_ = false; 
				return previous_(root_, result->get_key());
			} else {
			 	success_ = true; 
				return result->get_key();
			}
		}

		/**
		 * Восстановление ранее удаленного при вставке
		 */
		bool revive_(pnode & root, const T key)
		{
			if (!root) return false;

			if (root->get_key() == key)
			{
				if (root->isDeleted())
				{
					root->invertDel();
				} 

				return true;
			} else if (root->get_key() > key) {
				return revive_(root->l, key);
			} else if (root->get_key() < key)
				return revive_(root->r, key);

			return false;	
		}

		/**
		 * Вариант find_, игнорирующий удалённые soft'ом ключи
		 */
		bool was_(pnode & root, const T key)
		{
			if (!root) return false;

			if (root->get_key() == key) return true;
			if (root->get_key() > key) return was_(root->l, key);
			if (root->get_key() < key) return was_(root->r, key);
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
#ifdef DEBUG
            assert(count >= 0);
#endif
			if (!root)
				return std::make_pair(nullptr, nullptr);

			size_t left_size = sOf_(root->l);
            if (left_size >= count)
            {
                std::pair<pnode, pnode> res = _split_(root->l, count);
                root->l = res.second;
                update_count_(root);
                return std::make_pair(res.first, root);
            } else
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
		pnode _index_in_rope (pnode & root, const size_t idx)
		{
			pnode current = root;
			while (current)
			{
				size_t sL = sOf_(current->l);
				if (sL == idx) {
					success_ = true;
					return current;
				}

				current = sL > idx ? current->l : current->r;

				if (idx > sL)
					idx -= sL + 1;
#ifdef DEBUG 
				assert(idx >= 0);
#endif		

			}

            success_ = false;
			return NULL;
		}

#ifdef CONTEST
        void result_(pnode noda) {
        	if (!noda) return;

        	result_(noda->l);
        	std::cout << noda->get_key() << " ";
        	result_(noda->r);
        }
#endif
		/**
		 *--------------------------------------------------------
		 *--------------------------------------------------------
		 */

		void up_size() { ++size_; }

		void down_size() 
		{ 
			if(size_ > 0) 
				--size_; 
#ifdef DEBUG 
			else 
				std::cerr << "down_size for size_ < 1"; 
#endif		
		}

		void up_deleted_count_() { ++deleted_count_; }

		void down_deleted_count_() 
		{ 
			if(deleted_count_ > 0) 
				--deleted_count_; 
#ifdef DEBUG 
			else 
				std::cerr << "down_size for deleted_count_ < 1"; 
#endif		
		}
	};
}

#endif 


#include <iostream>
#define TEST_REMOVE 1

using namespace bst;

treap<size_t> tree;

#ifdef FILE_INPUT 
	const char* in_name_file = {"input.txt"};
#endif		

#ifdef FILE_OUTPUT 
	const char* out_name_file = {"output.txt"};
#endif		

int main(int argc, char const *argv[])
{
	size_t SIZE_ARR, OPERATION_COUNT;
	size_t left_bound, right_bound;

	std::cin >> SIZE_ARR >> OPERATION_COUNT;

	for (size_t i = 1; i <= SIZE_ARR; ++i)
	{
		tree.insert(i);
	}

	for (size_t i = 0; i < OPERATION_COUNT; ++i)
	{
		std::cin >> left_bound >> right_bound;
		tree.replace_in_head(left_bound, right_bound);
	}

	tree.contest_output();

}
