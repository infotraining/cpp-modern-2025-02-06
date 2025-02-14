#ifndef FWD_LIST_HPP
#define FWD_LIST_HPP

#include <memory>
#include <sstream>

namespace LegacyCode
{
    template <typename T>
    class FwdList
    {
    private:
        struct Node
        {
            T value;
            Node* next;
        };
    public:
        FwdList() : size_(0), head_(NULL)
        {
        }

        ~FwdList()
        {
            while (head_)
            {
                Node* node = head_;
                head_ = node->next;
                delete node;
            }
        }

        bool empty() const
        {
            return size_ == 0;
        }

        size_t size() const
        {
            return size_;
        }

        void push_front(const T& item)
        {
            Node* new_node = new Node{ item, head_ };
            head_ = new_node;
            ++size_;
        }

        T& front() const
        {
            assert(head_ != NULL);
            return head_->value;
        }

        void pop_front()
        {
            assert(size_ != 0);

            Node* node_to_pop = head_;
            head_ = node_to_pop->next;

            delete node_to_pop;
            --size_;
        }

        friend std::ostream& operator<<(std::ostream& out, const FwdList<T>& lst)
        {
            out << "[";
            Node* node = lst.head_;

            while(node)
            {
                out << node->value;
                node = node->next;

                if (node)
                    out << ", ";
            }

            out << "]";

            return out;
        }
    private:
        size_t size_;
        Node* head_;

        // FwdList is non-copyable
        FwdList(const FwdList&);
        FwdList& operator=(const FwdList&);
    };
}

namespace ModernCpp
{
    template <typename T>
    class FwdList
    {
    private:
        struct Node
        {
            T value;
            std::unique_ptr<Node> next;
        };
    public:
        FwdList()  = default;  

        FwdList(const FwdList&) = delete;
        FwdList& operator=(const FwdList&) = delete;
        FwdList(FwdList&&) = default;
        FwdList& operator=(FwdList&&) = default;
        
        ~FwdList() noexcept
        {
            while (head_)
            {
                head_ = std::move(head_->next);
            }
        }

        bool empty() const
        {
            return size_ == 0;
        }

        size_t size() const
        {
            return size_;
        }

        void push_front(const T& item)
        {
            auto new_node = std::make_unique<Node>(item, std::move(head_));
            head_ = std::move(new_node);
            ++size_;
        }

        T& front() const
        {
            assert(head_ != nullptr);
            return head_->value;
        }

        void pop_front()
        {
            assert(size_ != 0);

            head_ = std::move(head_->next);

            --size_;
        }

        friend std::ostream& operator<<(std::ostream& out, const FwdList<T>& lst)
        {
            out << "[";
            const auto* node = lst.head_.get();

            while(node)
            {
                out << node->value;
                node = node->next.get();

                if (node)
                    out << ", ";
            }

            out << "]";

            return out;
        }
    private:
        size_t size_{0};
        std::unique_ptr<Node> head_{nullptr};
    };
}

#endif