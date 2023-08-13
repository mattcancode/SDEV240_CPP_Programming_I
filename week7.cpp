#include <algorithm>
#include <iostream>
#include <list>

// simple helper function for dumping the list to the console
void print(const std::list<std::string>& list, const char* title = NULL)
{
    std::cout << (title != NULL ? title : "list") << ":\n ";

    for (const std::string& str : list)
    {
        std::cout << " " << str;
    }

    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    // Since I've mostly been using std::vector throughout previous weeks,
    // I thought it would be good to switch it up a bit and use std::list.
    // Unlike the vector, which is effectively just a mutable array so has
    // similar performance characteristics (fast append unless underlying
    // array capacity is exceeded, slow insert anywhere other than at the end,
    // constant random access, linear search), the list is implemented as a
    // linked list. This structure guarantees fast and constant inserts at the
    // front or back of the list (and anywhere in between, though requires
    // an iterator to identify the insertion point). It also has linear search
    // performance. However, random access is linear since you must iterate
    // through the list to get to the preferred index. Below are some of the
    // methods and algorithms included with in the list.

    // items can be inserted at initialization
    std::list<std::string> words = { "just", "some", "words", "in", "the", "list" };
    print(words, "after initialization");

    // items can be inserted later at the front of the list
    words.push_front("front");
    print(words, "with new word at the front");

    // and at the back of the list
    words.push_back("back");
    print(words, "with new word at the back");

    // items can also be removed from the front and back
    words.pop_front();
    words.pop_back();
    print(words, "words with front and back removed");

    // items can also be sorted - by default it will use the < operator
    words.sort();
    print(words, "sorted in default order");

    // lists can also be merged (it will attempt to insert in natural order)
    std::list<std::string> others = { "additional", "items", "merged" };
    words.merge(others);
    print(words, "words with other list merged");

    // they can also be sort using a user-defined comparison (this particular
    // example implements the comparator using an inline lambda function only
    // to keep the code together - normally, I would implement that elsewhere
    // for readability and reusability)
    words.sort([](const std::string& str1, const std::string& str2)
        {
            // if lengths are the same, use default order
            if (str1.length() == str2.length()) {
                return str1 < str2;
            }
            // otherwise shorter one goes first
            return str1.length() < str2.length();
        });
    print(words, "sorted by length");

    // list comes with a method to reverse the list
    words.reverse();
    print(words, "reversed");

    // the list can be resized, which will truncate if smaller than the list
    // or pad if longer (either with default value or value provided)
    words.resize(5);
    print(words, "truncated to 5 words");

    words.resize(8, "xtra");
    print(words, "expanded to 8");

    // STL includes a number of algorithms that can be applied to lists
    // one such algorithm is find, which is obviously useful for searching

    auto foundIter = std::find(words.begin(), words.end(), "items");
    std::cout << "'items' " << (foundIter == words.end() ? "not found" : "found" ) << std::endl;

    foundIter = std::find(words.begin(), words.end(), "missing");
    std::cout << "'missing' " << (foundIter == words.end() ? "not found" : "found" ) << std::endl;

    // if you need to know the minimum and/or maximum value in the list, you
    // could iterate through the list to determine that, or you could turn to
    // the STL library and use min_element, max_element, or minmax_element

    auto minmax = std::minmax_element(words.begin(), words.end());
    std::cout << "minumum value is " << *minmax.first << ", maximum is " << *minmax.second << std::endl;

    // finally, we can clear the list entirely, which might be useful if you
    // wanted to reuse a list to avoid allocating an entirely new one
    words.clear();
    print(words, "cleared");

    return 0;
}
