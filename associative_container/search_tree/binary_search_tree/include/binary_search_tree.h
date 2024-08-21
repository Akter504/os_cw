#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <optional>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <typename_holder.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>

template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:
    
    struct node
    {
    
    public:
        
        tkey _key;
        
        tvalue _value;
        
        node *left_subtree;
        
        node *right_subtree;
    
    public:
        
        explicit node(
            tkey const &key,
            tvalue const &value);
        
        explicit node(
            tkey const &key,
            tvalue &&value);
        
        virtual ~node() noexcept = default;
    };


public:
    
    // region iterators definition

    struct iterator_data
    {
    
    friend void binary_search_tree<tkey, tvalue>::inject_additional_data(
        iterator_data *,
        const node *) const;

    protected:
        
        bool _is_state_initialized = false;

        unsigned int _depth;
        
        tkey* _key;
        
        tvalue* _value;
    
    public:

        explicit iterator_data();
    
        explicit iterator_data(
            unsigned int depth,
            tkey const &key,
            tvalue const &value);

        explicit iterator_data(
            unsigned int depth,
            node** node);

        inline tkey const &get_key() const;

        inline tvalue const &get_value() const;
        
        inline unsigned int get_depth() const;
        

    };
    
    class Iterator
    {
        private:
            std::stack<node*> _state;
            iterator_data *_data;
            binary_search_tree<tkey, tvalue> const *_node;
        public:
            ~Iterator() noexcept;
            Iterator(Iterator& other);
            Iterator(Iterator&& other) noexcept;
            Iterator &operator=(Iterator& other);
            Iterator &operator=(Iterator&& other) noexcept;


        
    };

    class prefix_iterator final
    {

    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    
    public:
        
        explicit prefix_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_iterator const &other) const noexcept;
        
        prefix_iterator &operator++();
        
        prefix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_iterator final
    {
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit prefix_const_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_iterator const &other) const noexcept;
        
        prefix_const_iterator &operator++();
        
        prefix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class prefix_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit prefix_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_reverse_iterator const &other) const noexcept;
        
        prefix_reverse_iterator &operator++();
        
        prefix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class prefix_const_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit prefix_const_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_const_reverse_iterator const &other) const noexcept;
        
        prefix_const_reverse_iterator &operator++();
        
        prefix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit infix_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_iterator const &other) const noexcept;
        
        infix_iterator &operator++();
        
        infix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit infix_const_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
        
        explicit infix_const_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            std::stack<node*> const &state);
    
    public:
        
        bool operator==(
            infix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_iterator const &other) const noexcept;
        
        infix_const_iterator &operator++();
        
        infix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class infix_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit infix_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_reverse_iterator const &other) const noexcept;
        
        infix_reverse_iterator &operator++();
        
        infix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class infix_const_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit infix_const_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            infix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            infix_const_reverse_iterator const &other) const noexcept;
        
        infix_const_reverse_iterator &operator++();
        
        infix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit postfix_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_iterator const &other) const noexcept;
        
        postfix_iterator &operator++();
        
        postfix_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit postfix_const_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_const_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_iterator const &other) const noexcept;
        
        postfix_const_iterator &operator++();
        
        postfix_const_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    class postfix_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit postfix_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_reverse_iterator const &other) const noexcept;
        
        postfix_reverse_iterator &operator++();
        
        postfix_reverse_iterator const operator++(
            int not_used);
        
        iterator_data *operator*() const;
        
    };
    
    class postfix_const_reverse_iterator final
    {
    
    private:
        iterator_data *_data;
        std::stack<node*> _state;
        binary_search_tree<tkey, tvalue> const *_holder;
        friend void assign_data();

    public:
        
        explicit postfix_const_reverse_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            typename binary_search_tree<tkey, tvalue>::node *subtree_root);
    
    public:
        
        bool operator==(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        bool operator!=(
            postfix_const_reverse_iterator const &other) const noexcept;
        
        postfix_const_reverse_iterator &operator++();
        
        postfix_const_reverse_iterator const operator++(
            int not_used);
        
        iterator_data const *operator*() const;
        
    };
    
    // endregion iterators definition

protected:
    
    // region target operations strategies definition
    
    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };
    
    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };
    
    // endregion target operations strategies definition
    
    // region target operations associated exception types
    
    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    // endregion target operations associated exception types
    
    // region template methods definition
    
    class template_method_basics:
        public logger_guardant,
        private typename_holder
    {
    
    protected:
    
        binary_search_tree<tkey, tvalue> *_tree;
        
    public:
    
        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);
        
    protected:
        
        std::stack<node**> find_path(
            tkey const &key) const noexcept
        {
            std::stack<node **> result_path;

            node **path_finder = &(_tree->_root);
            auto const &comparer = _tree->_keys_comparer;

            while (true)
            {
                result_path.push(path_finder);

                if (*path_finder == nullptr)
                {
                    break;
                }

                auto comparison_result = comparer(key, (*path_finder)->_key);
                if (comparison_result == 0)
                {
                    break;
                }

                path_finder = comparison_result < 0
                    ? &((*path_finder)->left_subtree)
                    : &((*path_finder)->right_subtree);
            }

            return result_path;
        }

        virtual void balance(
            std::stack<node**> &path,
            node* node_to_dispose = nullptr);

    protected:
    
        [[nodiscard]] logger *get_logger() const noexcept final;
    private:
        inline std::string get_typename() const noexcept override;
        
    };
    
    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    public:
        
        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree);
        
    public:
        
        void insert(
            tkey const &key,
            tvalue const &value);
        
        void insert(
            tkey const &key,
            tvalue &&value);
    

    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        inline std::string get_typename() const noexcept override;
        
    };
    
    class obtaining_template_method:
        public template_method_basics
    {
    
    public:
        
        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);
    
    public:
        
        tvalue const &obtain(
            tkey const &key);
        
        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive);
    private:
        inline std::string get_typename() const noexcept override;
        
    };
    
    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {
    
    public:
        
        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree);
        
    public:
        
        tvalue dispose(
            tkey const &key);
    private:
        
        [[nodiscard]] allocator *get_allocator() const noexcept final;
        inline std::string get_typename() const noexcept override;
        
    };
    
    // endregion template methods definition

private:
    
    node *_root;
    
    insertion_template_method *_insertion_template = nullptr;
    
    obtaining_template_method *_obtaining_template = nullptr;
    
    disposal_template_method *_disposal_template = nullptr;

protected:
    
    explicit binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)>,
        allocator *allocator,
        logger *logger);

public:
    
    explicit binary_search_tree(
        std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);
    
    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;
    
    ~binary_search_tree() override;

public:
    void insert(
        tkey const &key,
        tvalue const &value) final;
    
    void insert(
        tkey const &key,
        tvalue &&value) final;
    
    tvalue const &obtain(
        tkey const &key) final;
    
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;
    
    tvalue dispose(
        tkey const &key) final;
    
public:
    
    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;
    
    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:
    
    // region iterators requests definition
    
    prefix_iterator begin_prefix() const noexcept;
    
    prefix_iterator end_prefix() const noexcept;
    
    prefix_const_iterator cbegin_prefix() const noexcept;
    
    prefix_const_iterator cend_prefix() const noexcept;
    
    prefix_reverse_iterator rbegin_prefix() const noexcept;
    
    prefix_reverse_iterator rend_prefix() const noexcept;
    
    prefix_const_reverse_iterator crbegin_prefix() const noexcept;
    
    prefix_const_reverse_iterator crend_prefix() const noexcept;
    
    infix_iterator begin_infix() const noexcept;
    
    infix_iterator end_infix() const noexcept;
    
    infix_const_iterator cbegin_infix() const noexcept;
    
    infix_const_iterator cend_infix() const noexcept;
    
    infix_reverse_iterator rbegin_infix() const noexcept;
    
    infix_reverse_iterator rend_infix() const noexcept;
    
    infix_const_reverse_iterator crbegin_infix() const noexcept;
    
    infix_const_reverse_iterator crend_infix() const noexcept;
    
    postfix_iterator begin_postfix() const noexcept;
    
    postfix_iterator end_postfix() const noexcept;
    
    postfix_const_iterator cbegin_postfix() const noexcept;
    
    postfix_const_iterator cend_postfix() const noexcept;
    
    postfix_reverse_iterator rbegin_postfix() const noexcept;
    
    postfix_reverse_iterator rend_postfix() const noexcept;
    
    postfix_const_reverse_iterator crbegin_postfix() const noexcept;
    
    postfix_const_reverse_iterator crend_postfix() const noexcept;
    
    // endregion iterators requests definition

protected:
    
    // region subtree rotations definition
    
    void small_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void small_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void big_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate = true) const;
    
    void double_left_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    void double_right_rotation(
        typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate = true) const;
    
    // endregion subtree rotations definition
    

protected:
    virtual iterator_data *create_iterator_data() const;
    virtual iterator_data *create_iterator_data(
        unsigned int size, node*& node) const;
    void clear(
        node **subtree_root);
    node *copy(
        node const *subtree_root);
    virtual inline size_t get_node_size() const noexcept;
    virtual inline void update_node_data(node *node) const noexcept;

    inline std::string get_typename() const noexcept override;
    void assign_data();

    virtual inline void call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue const &value);
    
    virtual inline void call_node_constructor(
        node *raw_space,
        tkey const &key,
        tvalue &&value);

    virtual void inject_additional_data(
            node *destination,
            node const *source) const;
    
    virtual void inject_additional_data(
            iterator_data *destination,
            node const *source) const;


// region binary_search_tree<tkey, tvalue>::node methods implementation

};

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value):
        _key(key),
        _value(value),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

};

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value):
        _key(key),
        _value(std::move(value)),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

};

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data()
{
    try
    {
        _key = reinterpret_cast<tkey*>(::operator new(sizeof(tkey*)));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw;
    }

    try
    {
        _value = reinterpret_cast<tvalue*>(::operator new(sizeof(tvalue*)));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        ::operator delete(_key);
        throw;
    }
    
    
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value):
    iterator_data()
{
    _key = reinterpret_cast<tkey *>(::operator new(sizeof(tkey), std::nothrow));
    _value = reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue), std::nothrow));
    allocator::construct(_key, key);
    allocator::construct(_value, value);
    
    _depth = depth;
    _is_state_initialized = true;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    node** node):
    _key(nullptr),
    _value(nullptr),
    _is_state_initialized(false),
    _depth(depth)
{
    if ( (*node) != nullptr)
    {
        _key = &((*node)->_key);
        _value = &((*node)->_value);
        _is_state_initialized = true;
    }
}

template<
    typename tkey,
    typename tvalue>
inline tkey const &binary_search_tree<tkey, tvalue>::iterator_data::get_key() const
{
    if (_is_state_initialized)
    {
        return *_key;
    }
    throw std::logic_error("In get_key: Iterator data not be initialized");
}

template<
    typename tkey,
    typename tvalue>
inline tvalue const &binary_search_tree<tkey, tvalue>::iterator_data::get_value() const
{
    if (_is_state_initialized)
    {
        if (_value != nullptr)
        {
            return *_value;
        }
    }
    throw std::logic_error("In get_value: Iterator data not be initialized");
}

template<
    typename tkey,
    typename tvalue>
inline unsigned int binary_search_tree<tkey, tvalue>::iterator_data::get_depth() const
{
    if (_is_state_initialized)
    {
        return _depth;
    }
    throw std::logic_error("In get_depth: Iterator data not be initialized");
}

// endregion iterator data implementation

// region initialization iterator

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey,tvalue>::Iterator::Iterator(Iterator& other):
    _state(other._state),
    _node(other._node)
{ 
    if (_state.empty())
    {
        _data = _node->create_iterator_data();
    }
    else
    {
        _data = _node->create_iterator_data(_state.size() - 1, _state.top());
    }
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::Iterator &binary_search_tree<tkey, tvalue>::Iterator::operator=(Iterator& other)
{ 
    if (this != &other)
    {
        _state = other._state;
        _node = other._node;

        if (_state.empty())
        {
            _data = _node->create_iterator_data();
        }
        else
        {
            _data = _node->create_iterator_data(_state.size() - 1, _state.top());
        }
    }
}

// endregion initialization iterator

// region prefix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.top() == nullptr)
    {
        return *this;
    }

    node* node_tree = _state.top();

    if (node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
    }
    else if(node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
    }
    else
    {
        node* old_node = node_tree;
        _state.pop();

        if (!_state.empty())
        {
            while (true)
            {
                if (_state.empty())
                {
                    break;
                }
                node_tree = _state.top();
                if (node_tree->right_subtree != nullptr && node_tree->right_subtree != old_node)
                {
                    _state.push(node_tree->right_subtree);
                    this->_data = _holder->create_iterator_data(this->_state.size()-1, node_tree->right_subtree);
                    break;
                }
                else
                {
                    old_node = node_tree;
                    _state.pop();
                }
            }

        }
        return *this;
    }
    this->_data = _holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    return this->_data;
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return (*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.top() == nullptr)
    {
        return *this;
    }

    node* node_tree = _state.top();

    if (node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
    }
    else if(node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
    }
    else
    {
        node* old_node = node_tree;
        _state.pop();

        if (!_state.empty())
        {
            while (true)
            {
                if (_state.empty())
                {
                    break;
                }
                node_tree = _state.top();
                if (node_tree->right_subtree != nullptr && node_tree->right_subtree != old_node)
                {
                    _state.push(node_tree->right_subtree);
                    this->_data = _holder->create_iterator_data(this->_state.size()-1, node_tree->right_subtree);
                    break;
                }
                else
                {
                    old_node = node_tree;
                    _state.pop();
                }
            }

        }
        return *this;
    }
    this->_data = _holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    return this->_data;
}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return (*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.top() == nullptr)
    {
        return *this;
    }

    node* node_tree = _state.top();

    if (node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
    }
    else if(node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
    }
    else
    {
        node* old_node = node_tree;
        _state.pop();

        if (!_state.empty())
        {
            while (true)
            {
                if (_state.empty())
                {
                    break;
                }
                node_tree = _state.top();
                if (node_tree->left_subtree != nullptr && node_tree->left_subtree != old_node)
                {
                    _state.push(node_tree->left_subtree);
                    this->_data = _holder->create_iterator_data(this->_state.size()-1, node_tree->left_subtree);
                    break;
                }
                else
                {
                    old_node = node_tree;
                    _state.pop();
                }
            }

        }
        return *this;
    }
    this->_data = _holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return (*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.top() == nullptr)
    {
        return *this;
    }

    node* node_tree = _state.top();

    if (node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
    }
    else if(node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
    }
    else
    {
        node* old_node = node_tree;
        _state.pop();

        if (!_state.empty())
        {
            while (true)
            {
                if (_state.empty())
                {
                    break;
                }
                node_tree = _state.top();
                if (node_tree->left_subtree != nullptr && node_tree->left_subtree != old_node)
                {
                    _state.push(node_tree->left_subtree);
                    this->_data = _holder->create_iterator_data(this->_state.size()-1, node_tree->left_subtree);
                    break;
                }
                else
                {
                    old_node = node_tree;
                    _state.pop();
                }
            }

        }
        return *this;
    }
    this->_data = _holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        while (temp != nullptr)
        {
            this->_state.push(temp);
            temp = temp->left_subtree;
        }
    }

    temp = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return *this;
    }

    node* node_tree = _state.top();
    if (node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
        node_tree = node_tree->right_subtree;
        while (node_tree->left_subtree != nullptr)
        {
            _state.push(node_tree->left_subtree);
            node_tree = node_tree->left_subtree;
        }
        if (node_tree != nullptr)
        {
            this->_data = _holder->create_iterator_data(_state.size()-1, node_tree);
        }
        return *this;
    }

    node* old_node;
    while (true)
    {
        old_node = node_tree;
        _state.pop();
        if (_state.empty())
        {
            return *this;
        }
        node_tree = _state.top();
        if (node_tree->left_subtree == old_node)
        {
            break;
        }
    }
    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    return this->_data;
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        while (temp != nullptr)
        {
            this->_state.push(temp);
            temp = temp->left_subtree;
        }
    }

    temp = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
            binary_search_tree<tkey, tvalue> const *holder,
            std::stack<node*> const &state):
    _holder(holder),
    _state(state)
{};

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return *this;
    }

    node* node_tree = _state.top();
    if (node_tree->right_subtree != nullptr)
    {
        _state.push(node_tree->right_subtree);
        node_tree = node_tree->right_subtree;
        while (node_tree->left_subtree != nullptr)
        {
            _state.push(node_tree->left_subtree);
            node_tree = node_tree->left_subtree;
        }
        if (node_tree != nullptr)
        {
            this->_data = _holder->create_iterator_data(_state.size()-1, node_tree);
        }
        return *this;
    }

    node* old_node;
    while (true)
    {
        old_node = node_tree;
        _state.pop();
        if (_state.empty())
        {
            return *this;
        }
        node_tree = _state.top();
        if (node_tree->left_subtree == old_node)
        {
            break;
        }
    }
    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    return this->_data;
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        while (temp != nullptr)
        {
            this->_state.push(temp);
            temp = temp->right_subtree;
        }
    }

    temp = this->_state.empty() ? nullptr : this->_state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return !(*this != other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return *this;
    }

    node* node_tree = _state.top();
    if (node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
        node_tree = node_tree->left_subtree;
        while (node_tree->right_subtree != nullptr)
        {
            _state.push(node_tree->right_subtree);
            node_tree = node_tree->right_subtree;
        }
        if (node_tree != nullptr)
        {
            this->_data = _holder->create_iterator_data(_state.size()-1, node_tree);
        }
        return *this;
    }

    node* old_node;
    while (true)
    {
        old_node = node_tree;
        _state.pop();
        if (_state.empty())
        {
            return *this;
        }
        node_tree = _state.top();
        if (node_tree->right_subtree == old_node)
        {
            break;
        }
    }
    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    auto &_state = this->_state;
    if (_state.empty())
    {
        return *this;
    }

    node* node_tree = _state.top();
    if (node_tree->left_subtree != nullptr)
    {
        _state.push(node_tree->left_subtree);
        node_tree = node_tree->left_subtree;
        while (node_tree->right_subtree != nullptr)
        {
            _state.push(node_tree->right_subtree);
            node_tree = node_tree->right_subtree;
        }
        if (node_tree != nullptr)
        {
            this->_data = _holder->create_iterator_data(_state.size()-1, node_tree);
        }
        return *this;
    }

    node* old_node;
    while (true)
    {
        old_node = node_tree;
        _state.pop();
        if (_state.empty())
        {
            return *this;
        }
        node_tree = _state.top();
        if (node_tree->right_subtree == old_node)
        {
            break;
        }
    }
    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        while (temp != nullptr)
        {
            this->_state.push(temp);
            temp = temp->right_subtree;
        }
    }
    temp = this->_state.empty() ? nullptr: _state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    auto& _state = this->_state;
    if (_state.empty())
    {
        return *this;
    }   

    node* old_node = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }   
    node* new_node = _state.top();
    
    if (new_node->right_subtree == old_node || new_node->right_subtree == nullptr)
    {
        this->_data = this->_holder->create_iterator_data(this->_state.size()-1, new_node);
        return *this;
    }

    new_node = new_node->right_subtree;
    while (new_node != nullptr)
    {
        _state.push(new_node);
        new_node = new_node->left_subtree != nullptr 
            ? new_node->left_subtree
            : new_node->right_subtree;
    }

    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    return this->_data;
}
    

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        while (temp != nullptr)
        {
            this->_state.push(temp);
            if (temp->left_subtree == nullptr)
            {
                temp = temp->right_subtree;
            }
            else
            {
                temp = temp->left_subtree;
            }
        }
    }
    temp = this->_state.empty() ? nullptr: _state.top();
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
    auto& _state = this->_state;
    if (_state.empty())
    {
        return *this;
    }   

    node* old_node = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }   
    node* new_node = _state.top();
    
    if (new_node->right_subtree == old_node || new_node->right_subtree == nullptr)
    {
        this->_data = this->_holder->create_iterator_data(this->_state.size()-1, new_node);
        return *this;
    }

    new_node = new_node->right_subtree;
    while (new_node != nullptr)
    {
        _state.push(new_node);
        new_node = new_node->left_subtree != nullptr 
            ? new_node->left_subtree
            : new_node->right_subtree;
    }

    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;

}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    return this->_data;
}
// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    auto& _state = this->_state;
    if (_state.empty())
    {
        return *this;
    }   

    node* old_node = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }   
    node* new_node = _state.top();
    
    if (new_node->left_subtree == old_node || new_node->left_subtree == nullptr)
    {
        this->_data = this->_holder->create_iterator_data(this->_state.size()-1, new_node);
        return *this;
    }

    new_node = new_node->left_subtree;
    while (new_node != nullptr)
    {
        _state.push(new_node);
        new_node = new_node->right_subtree != nullptr 
            ? new_node->right_subtree
            : new_node->left_subtree;
    }

    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(
    binary_search_tree<tkey, tvalue> const *holder,
    typename binary_search_tree<tkey, tvalue>::node *subtree_root)
{
    this->_holder = holder;
    node* temp = subtree_root;
    if (temp != nullptr)
    {
        this->_state.push(temp);
    }
    this->_data = holder->create_iterator_data(this->_state.size() - 1, temp);
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    if (this->_state.empty() && other._state.empty())
    {
        return true;
    }
    if (this->_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return this->_state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
    typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    auto& _state = this->_state;
    if (_state.empty())
    {
        return *this;
    }   

    node* old_node = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }   
    node* new_node = _state.top();
    
    if (new_node->left_subtree == old_node || new_node->left_subtree == nullptr)
    {
        this->_data = this->_holder->create_iterator_data(this->_state.size()-1, new_node);
        return *this;
    }

    new_node = new_node->left_subtree;
    while (new_node != nullptr)
    {
        _state.push(new_node);
        new_node = new_node->right_subtree != nullptr 
            ? new_node->right_subtree
            : new_node->left_subtree;
    }

    this->_data = this->_holder->create_iterator_data(this->_state.size()-1, _state.top());
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
    int not_used)
{
    auto it = *this;
    ++(*this);
    return it;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
    return this->_data;
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to insert already existing key inside the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{

}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{

}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
    binary_search_tree<tkey, tvalue> *tree)
    : _tree(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::template_method_basics::balance(
    std::stack<node**> &path,
    node* node_to_dispose)
{ }

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return this->_tree->get_logger();
}

template<
    typename tkey,
    typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::template_method_basics::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::template_method_basics";
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue const &value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (this->_tree->_insertion_strategy)
        {
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->_value = value;
                break;
        }

        return;
    }

    *(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1));
    this->_tree->call_node_constructor(*(path.top()), key, value);
    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (this->_tree->_insertion_strategy)
        {
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->_value = std::move(value);
                break;
        }

        return;
    }

    *(path.top()) = reinterpret_cast<node *>(allocate_with_guard(this->_tree->get_node_size(), 1));
    this->_tree->call_node_constructor(*(path.top()), key, std::move(value));
    this->balance(path);
}

template<
    typename tkey,
    typename tvalue>
allocator *binary_search_tree<tkey, tvalue>::insertion_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

template<
    typename tkey,
    typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::insertion_template_method::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::insertion_template_method";
}

// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
    tkey const &key)
{
    std::stack<node**> path = this->find_path(key);
    if ((*path.top()) == nullptr)
    {
        throw typename search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception(key);
    }

    return (*(path.top()))->_value;

}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;
    std::stack<node*> path;
    node *path_finder = this->_tree->_root;
    auto const &comparer = this->_tree->_keys_comparer;

    while (true)
    {
        path.push(path_finder);

        if (path_finder == nullptr)
        {
            break;
        }

        auto comparison_result = comparer(lower_bound, path_finder->_key);
        if (comparison_result == 0)
        {
            break;
        }

        path_finder = comparison_result < 0
            ? (path_finder->left_subtree)
            : (path_finder->right_subtree);
    }

    if (path.empty())
    {
        return range;
    }
    
    if (lower_bound_inclusive)
    {
        node *top = path.top();
        range.push_back({ top->_key, top->_value });
    }

    auto iter = binary_search_tree<tkey,tvalue>::infix_const_iterator(this->_tree, path);
    ++iter;

    while(iter != this->_tree->cend_infix() && 
            (comparer((*iter)->get_key(), upper_bound)) < (upper_bound_inclusive ? 1 : 0))
    {
        range.push_back({ (*iter)->get_key(), (*iter)->get_value() });
        std::cout << "key: " << (*iter)->get_key() << " " << "value: " << (*iter)->get_value() << std::endl;
        ++iter;
    }
    
    return range;
}

template<
    typename tkey,
    typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::obtaining_template_method::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::obtaining_template_method";
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
    binary_search_tree<tkey, tvalue> *tree):
    binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
    tkey const &key)
{
    std::stack<node**> path = this->find_path(key);

    if (path.empty() || *(path.top()) == nullptr)
    {
        switch (this->_tree->_disposal_strategy)
        {
            case search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw typename search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
            case search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }
    node* target_node = *path.top();
    tvalue node_value = target_node->_value;
    path.pop();
    node* parent = nullptr;
    if (!path.empty())
    {
        parent = *(path.top());
    }

    if (target_node->left_subtree == nullptr && target_node->right_subtree == nullptr)
    {
        if (parent != nullptr && parent->left_subtree == target_node)
        {
            parent->left_subtree = nullptr;
        }
        else if (parent != nullptr && parent->right_subtree == target_node)
        {
            parent->right_subtree = nullptr;
        }
        deallocate_with_guard(target_node);
    }
    else if (target_node->left_subtree != nullptr && target_node->right_subtree == nullptr)
    {
        if (parent != nullptr && parent->left_subtree == target_node)
        {
            parent->left_subtree = target_node->left_subtree;
        }
        else if (parent != nullptr && parent->right_subtree == target_node)
        {
            parent->right_subtree = target_node->left_subtree;
        }

        if (parent == nullptr)
        {
            this->_tree->_root = target_node->left_subtree;
        }
        path.push(&target_node->left_subtree);
        deallocate_with_guard(target_node);
    }
    else if (target_node->left_subtree == nullptr && target_node->right_subtree != nullptr)
    {
        if (parent != nullptr && parent->left_subtree == target_node)
        {
            parent->left_subtree = target_node->right_subtree;
        }
        else if (parent != nullptr && parent->right_subtree == target_node)
        {
            parent->right_subtree = target_node->right_subtree;
        }

        if (parent == nullptr)
        {
            this->_tree->_root = target_node->right_subtree;
        }
        path.push(&target_node->right_subtree);
        deallocate_with_guard(target_node);
    }
    else if (target_node->left_subtree != nullptr && target_node->right_subtree != nullptr)
    {
        node* temp = target_node;
        temp = temp->left_subtree;
        node* parent_temp = target_node;
        while (temp->right_subtree != nullptr)
        {
            parent_temp = temp;
            temp = temp->right_subtree;
        }
        if (temp->left_subtree != nullptr)
        {
            if (parent_temp != target_node)
            {
                parent_temp->right_subtree = temp->left_subtree;
                temp->left_subtree = target_node->left_subtree;
                temp->right_subtree = target_node->right_subtree;
            }
            else
            {
                temp->right_subtree = temp->left_subtree;
                temp->left_subtree = target_node->left_subtree;
            }

        }
        else
        {
            parent_temp->right_subtree = nullptr;
            temp->left_subtree = target_node->left_subtree;
            temp->right_subtree = target_node->right_subtree;
        }


        if (parent != nullptr && parent->left_subtree == target_node)
        {
            parent->left_subtree = temp;
        }
        else if (parent != nullptr && parent->right_subtree == target_node)
        {
            parent->right_subtree = temp;
        }


        if (parent == nullptr)
        {
            this->_tree->_root = temp;
        }
        path.push(&temp);
        deallocate_with_guard(target_node);

    }
    return node_value;


}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

template<
    typename tkey,
    typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::disposal_template_method::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>::disposal_template_method";
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region additional functions

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::create_iterator_data() const
{
    return new iterator_data;
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::create_iterator_data(unsigned int depth, node*& node) const
{
    return new iterator_data(depth, &node);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::clear(
    node **subtree_root)
{
    if ((*subtree_root) == nullptr)
    {
        return;
    }    

    clear(&(*subtree_root)->left_subtree);
    clear(&(*subtree_root)->right_subtree);

    (*subtree_root)->~node();
    this->deallocate_with_guard(*subtree_root);
    *subtree_root = nullptr;


}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::node *binary_search_tree<tkey, tvalue>::copy(
    node const *subtree_root)
{
    if (subtree_root == nullptr)
    {
        return nullptr;
    }
    
    node *subtree_root_copied = nullptr;
    
    try
    {
        subtree_root_copied = reinterpret_cast<node*>(this->allocate_with_guard(this->get_node_size(), 1));
        call_node_constructor(subtree_root_copied, subtree_root->_key, subtree_root->_value);
        inject_additional_data(subtree_root_copied, subtree_root);
        subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
        subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);
    }
    catch (const std::bad_alloc& ex)
    {
        clear(&subtree_root_copied);
        this->error_with_guard(get_typename() + "::copy(node const *) : bad alloc occurred while copying node.");
        throw;
    }
    
    return subtree_root_copied;
}

template<
    typename tkey,
    typename tvalue>
inline size_t binary_search_tree<tkey, tvalue>::get_node_size() const noexcept
{
    return sizeof(typename binary_search_tree<tkey, tvalue>::node);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::update_node_data(node *node) const noexcept
{ }

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
    node *raw_space,
    tkey const &key,
    tvalue const &value)
{
    allocator::construct(raw_space, key, value);
}

template<
    typename tkey,
    typename tvalue>
inline void binary_search_tree<tkey, tvalue>::call_node_constructor(
    node *raw_space,
    tkey const &key,
    tvalue &&value)
{
    allocator::construct(raw_space, key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::inject_additional_data(
    node *destination,
    node const *source) const
{ }

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::inject_additional_data(
    iterator_data *destination,
    node const *source) const
{ }

// end region additional functions

// region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
    typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
    typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
    std::function<int(tkey const &, tkey const &)> comparer,
    allocator *allocator,
    logger *logger):
    search_tree<tkey, tvalue>(comparer, logger, allocator),
    _root(nullptr),
    _insertion_template(insertion_template),
    _obtaining_template(obtaining_template),
    _disposal_template(disposal_template)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger,
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
    binary_search_tree(
        new binary_search_tree<tkey, tvalue>::insertion_template_method(this),
        new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
        new binary_search_tree<tkey, tvalue>::disposal_template_method(this),
        keys_comparer,
        allocator,
        logger)
{

}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> const &other)
{
    try
    {
        this->_root = copy(reinterpret_cast<node*>(other._root));
        _insertion_template = new binary_search_tree<tkey, tvalue>::insertion_template_method(this);
        _obtaining_template = new binary_search_tree<tkey, tvalue>::obtaining_template_method(this);
        _disposal_template = new binary_search_tree<tkey, tvalue>::disposal_template_method(this);
    }
    catch (const std::bad_alloc& ex)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    this->_root = other._root;
    
    _insertion_template = other._insertion_template;
    _obtaining_template = other._obtaining_template;
    _disposal_template = other._disposal_template;
    
    other._logger = nullptr;
    other._allocator = nullptr;
    other._root = nullptr;
    other._insertion_template = nullptr;
    other._obtaining_template = nullptr;
    other._disposal_template = nullptr;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        this->_root = copy(reinterpret_cast<node*>(other._root));
        
        this->_keys_comparer = other._keys_comparer;
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        
        this->_insertion_strategy = other._insertion_strategy;
        this->_disposal_strategy = other._disposal_strategy;
        
        *_insertion_template = *(other._insertion_template);
        *_obtaining_template = *(other._obtaining_template);
        *_disposal_template = *(other._disposal_template);
        
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
    binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear(reinterpret_cast<node**>(&this->_root));
        delete _insertion_template;
        delete _obtaining_template;
        delete _disposal_template;
        
        this->_keys_comparer = std::move(other._keys_comparer);
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_root = other._root;
        this->_insertion_strategy = other._insertion_strategy;
        this->_disposal_strategy = other._disposal_strategy;
        
        _insertion_template = other._insertion_template;
        _obtaining_template = other._obtaining_template;
        _disposal_template = other._disposal_template;
        
        other._allocator = nullptr;
        other._logger = nullptr;
        other._root = nullptr;
        other._insertion_template = nullptr;
        other._obtaining_template = nullptr;
        other._disposal_template = nullptr;
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    clear(reinterpret_cast<node**>(&this->_root));
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
}

template<
    typename tkey,
    typename tvalue>
inline std::string binary_search_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "binary_search_tree<tkey, tvalue>";
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation


template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    _insertion_template->insert(key, value);
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    _insertion_template->insert(key, std::move(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    return _obtaining_template->obtain(key);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : called.")
        ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : called.");
    
    auto values = _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
    
    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : successfuly finished.");
    return values;
}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    return _disposal_template->dispose(key);
}

// endregion associative_containers contract implementations

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this,nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this,nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this,nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, reinterpret_cast<typename binary_search_tree<tkey, tvalue>::node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this,nullptr);
}


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::small_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::big_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_left_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
    binary_search_tree<tkey, tvalue>::node *&subtree_root,
    bool at_grandparent_first,
    bool validate) const
{
    throw not_implemented("template<typename tkey, typename tvalue> void binary_search_tree<tkey, tvalue>::double_right_rotation(binary_search_tree<tkey, tvalue>::node *&, bool, bool) const", "your code should be here...");
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H