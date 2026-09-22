#include <mutex>
#ifndef lacpp_sorted_list_hpp
#define lacpp_sorted_list_hpp lacpp_sorted_list_hpp
#include <cstddef>

/* a sorted list implementation by David Klaftenegger, 2015
 * please report bugs or suggest improvements to david.klaftenegger@it.uu.se
 */

/* struct for list nodes */
template<typename T>
struct node {
	T value;
	node<T>* next;
};

template<typename T, typename lockType>
struct nodeWithLock {
	T value;
	nodeWithLock<T, lockType>* next;
	lockType mutex;
};

/* non-concurrent sorted singly-linked list */
template<typename T>
class sorted_list {
	node<T>* first = nullptr;

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list() = default;
		sorted_list(const sorted_list<T>& other) = default;
		sorted_list(sorted_list<T>&& other) = default;
		sorted_list<T>& operator=(const sorted_list<T>& other) = default;
		sorted_list<T>& operator=(sorted_list<T>&& other) = default;
		~sorted_list() {
			while(first != nullptr) {
				remove(first->value);
			}
		}
		/* insert v into the list */
		void insert(T v) {
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* succ = first;
			while(succ != nullptr && succ->value < v) {
				pred = succ;
				succ = succ->next;
			}
			
			/* construct new node */
			node<T>* current = new node<T>();
			current->value = v;

			/* insert new node between pred and succ */
			current->next = succ;
			if(pred == nullptr) {
				first = current;
			} else {
				pred->next = current;
			}
		}

		void remove(T v) {
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				pred = current;
				current = current->next;
			}
			if(current == nullptr || current->value != v) {
				/* v not found */
				return;
			}
			/* remove current */
			if(pred == nullptr) {
				first = current->next;
			} else {
				pred->next = current->next;
			}
			delete current;
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;
			/* first go to value v */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current = current->next;
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;
				current = current->next;
			}
			return cnt;
		}
};



/// Using course-grained locking 
template<typename T>
class sorted_list_c1 {
	node<T>* first = nullptr;
	std::mutex lock; 

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list_c1() = default;
		sorted_list_c1(const sorted_list_c1<T>& other) = default;
		sorted_list_c1(sorted_list_c1<T>&& other) = default;
		sorted_list_c1<T>& operator=(const sorted_list_c1<T>& other) = default;
		sorted_list_c1<T>& operator=(sorted_list_c1<T>&& other) = default;
		~sorted_list_c1() {
			while(first != nullptr) {
				remove(first->value);
			}
		}
		/* insert v into the list */
		void insert(T v) {
			const std::lock_guard<std::mutex> guard {lock};
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* succ = first;
			while(succ != nullptr && succ->value < v) {
				pred = succ;
				succ = succ->next;
			}
			
			/* construct new node */
			node<T>* current = new node<T>();
			current->value = v;

			/* insert new node between pred and succ */
			current->next = succ;
			if(pred == nullptr) {
				first = current;
			} else {
				pred->next = current;
			}
		}

		void remove(T v) {
			const std::lock_guard<std::mutex> guard {lock};
			/* first find position */
			node<T>* pred = nullptr;
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				pred = current;
				current = current->next;
			}
			if(current == nullptr || current->value != v) {
				/* v not found */
				return;
			}
			/* remove current */
			if(pred == nullptr) {
				first = current->next;
			} else {
				pred->next = current->next;
			}
			delete current;
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			const std::lock_guard<std::mutex> guard {lock};
			std::size_t cnt = 0;
			/* first go to value v */
			node<T>* current = first;
			while(current != nullptr && current->value < v) {
				current = current->next;
			}
			/* count elements */
			while(current != nullptr && current->value == v) {
				cnt++;
				current = current->next;
			}
			return cnt;
		}
};


/// Using fine-grained locking  
template<typename T>
class sorted_list_c2 {
	using nodeType = nodeWithLock<T, std::mutex>;
	// These are only sentinels.
	nodeType* first = nullptr;
	nodeType* last = nullptr;

	public:
		/* default implementations:
		 * default constructor
		 * copy constructor (note: shallow copy)
		 * move constructor
		 * copy assignment operator (note: shallow copy)
		 * move assignment operator
		 *
		 * The first is required due to the others,
		 * which are explicitly listed due to the rule of five.
		 */
		sorted_list_c2(T start_sentinel, T end_sentinel) {
			first = new nodeType();
			first->value = start_sentinel;

			last = new nodeType();
			last->value = end_sentinel;
			
			first->next = last;
		}
		sorted_list_c2(const sorted_list_c2<T>& other) = default;
		sorted_list_c2(sorted_list_c2<T>&& other) = default;
		sorted_list_c2<T>& operator=(const sorted_list_c2<T>& other) = default;
		sorted_list_c2<T>& operator=(sorted_list_c2<T>&& other) = default;
		~sorted_list_c2() {
			// while(first->next != nullptr) {
			// 	remove(first->value);
			// }

			auto current = first->next;
			while (current != last) {
				remove(current->value);
			}

			delete last;
			delete first;
		}
		/* insert v into the list */
		void insert(T v) {
			/* first find position */
			first->mutex.lock();
			nodeType* pred = first;
			nodeType* succ = pred->next;
			succ->mutex.lock();

			while(succ->value < v) {
				pred->mutex.unlock();
				pred = succ;
				succ = succ->next;
				succ->mutex.lock();
			}
			
			/* construct new node */
			nodeType* current = new nodeType();
			current->value = v;

			/* insert new node between pred and succ */
			current->next = succ;
			pred->next = current;

			succ->mutex.unlock();
			pred->mutex.unlock();
		}

		void remove(T v) {
			/* first find position */
			first->mutex.lock();
			nodeType* pred = first;
			nodeType* current = first->next;
			current->mutex.lock();

			while(current->value < v) {
				pred->mutex.unlock();
				pred = current;
				current = pred->next;
				current->mutex.lock();
			}

			// Note: we never need to worry about removing the first and last node containing sentinel values since 
			// v can never be any of the sentinels (except when the deconstructor is called, but at that point we do not care). 
			if(current->value == v) {
				/* v not found */

				// Something strange is happening here! Some free-after-use is cause by this.
				auto afterCurrent = current->next; 
				pred->next = afterCurrent;
				current->mutex.unlock();
				delete current;	
			} else {
				current->mutex.unlock();
			}
			
			pred->mutex.unlock();
		}

		/* count elements with value v in the list */
		std::size_t count(T v) {
			std::size_t cnt = 0;
			/* first go to value v */
			first->mutex.lock();
			nodeType* pred = first;
			nodeType* current = first->next;
			current->mutex.lock();

			while(current->value < v) {
				pred->mutex.unlock();
				pred = current;
				current = pred->next;
				current->mutex.lock();
			}

			/* count elements */
			while(current->value == v) {
				cnt++;
				pred->mutex.unlock();
				pred = current;
				current = current->next;
				current->mutex.lock();
			}
			
			current->mutex.unlock();
			pred->mutex.unlock();

			return cnt;
		}
};

#endif // lacpp_sorted_list_hpp
