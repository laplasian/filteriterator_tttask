#ifndef FILTERITERATOR_HPP
#define FILTERITERATOR_HPP

#include <iterator>
#include <type_traits>
#include <concepts>
#include <stdexcept>
#include <functional>

namespace iterator {
    namespace Impl {
    template<class Iterator>
    concept ValidIter = std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category> && requires(Iterator it)
    {
        ++it;
        *it;
        it == it;
    };

        template<class Iterator, class Predicate>
        class filter_iterator {
        public:
            using value_type        = typename std::iterator_traits<Iterator>::value_type;
            using reference         = typename std::iterator_traits<Iterator>::reference;
            using pointer           = typename std::iterator_traits<Iterator>::pointer;
            using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
            using iterator_category = std::forward_iterator_tag;

            filter_iterator() = default;
            filter_iterator(Iterator current, Iterator last, Predicate& pred): current_(current), last_(last), pred_(pred) {
                find_next_valid();
            }

            reference operator*() const {return *current_;}
            pointer operator->() const {return &(*current_);}

            filter_iterator& operator++() {
                if (current_ != last_) {
                    ++current_;
                    find_next_valid();
                }
                return *this;
            };
            filter_iterator  operator++(int) {
                filter_iterator tmp = *this;
                ++(*this);
                return tmp;
            };

            bool operator==(const filter_iterator& other) const noexcept {return current_ == other.current_;}
            bool operator!=(const filter_iterator& other) const noexcept{return !(*this==other);}

        private:
            void find_next_valid() {
                while (current_ != last_ && !pred_(*current_)) {
                    ++current_;
                }
            };

            Iterator current_{};
            Iterator last_{};
            Predicate& pred_;
        };
    }

    template<Impl::ValidIter Iterator>
    class filter_range {
    public:
        using Predicate = std::function<bool(const typename std::iterator_traits<Iterator>::value_type&)>;
        using iterator = Impl::filter_iterator<Iterator,Predicate>;

        filter_range(Iterator first, Iterator last, Predicate && pred): first_(first), last_{last}, pred_(std::move(pred)) {};

        iterator begin() noexcept {
            return Impl::filter_iterator(first_,last_,pred_);
        };
        iterator end() noexcept {
            return Impl::filter_iterator(last_,last_,pred_);
        };

    private:
        Iterator first_{};
        Iterator last_{};
        Predicate pred_;
    };
}

#endif //FILTERITERATOR_HPP
