#include <iostream>
#include <vector>

using std::string;

class Publication
{
private:
    struct Author
    {
        string firstName;
        string lastName;

        Author(string firstName, string lastName)
        {
            this->firstName = firstName;
            this->lastName = lastName;
        }
    };

    std::vector<Author> authors;

protected:
    string name;
    int price;

    void addAuthor(string firstName, string lastName)
    {
        authors.push_back(Author(firstName, lastName));
    }

public:
    double getPrice()
    {
        return price;
    }

    void setPrice(int price)
    {
        this->price = price;
    }

    void setAuthors(string firstName, string lastName)
    {
        authors.clear();
        addAuthor(firstName, lastName);
    }

    void setAuthors(string firstName1, string lastName1,
                    string firstName2, string lastName2)
    {
        setAuthors(firstName1, lastName1);
        addAuthor(firstName2, lastName2);
    }

    void setAuthors(string firstName1, string lastName1,
                    string firstName2, string lastName2,
                    string firstName3, string lastName3)
    {
        setAuthors(firstName1, lastName1, firstName2, lastName2);
        addAuthor(firstName3, lastName3);
    }

    string listAuthors()
    {
        string result;

        int remaining = authors.size();

        for (Author author : authors)
        {
            result.append(author.lastName);
            if (!author.firstName.empty())
            {
                result.append(", ").append(author.firstName, 0, 1).append(".");
            }

            if (--remaining > 0)
            {
                result += ", ";
            }
        }

        return result;
    }
};

class Book : public Publication
{
public:
    string isbn;
    int pages;
    int year;

    Book(int price)
    {
        this->price = price;
    }

    string getName()
    {
        return name;
    }

    void setName(string name)
    {
        this->name = name;
    }
};

class Journal : public Publication
{
public:
    string issn;

    string getName()
    {
        return name;
    }

    void setName(string name)
    {
        this->name = name;
    }
};

class Article : public Journal
{
public:
    int volume;
    int issue;
    int year;
    int firstPage;
    int lastPage;
    string title;

    Article(int price)
    {
        this->price = price;
    }

    string citation()
    {
        return string(listAuthors())
            .append(" (")
            .append(std::to_string(year))
            .append("). ")
            .append(title)
            .append(". ")
            .append(name)
            .append(", ")
            .append(std::to_string(volume))
            .append(", (")
            .append(std::to_string(issue))
            .append("), pages ")
            .append(std::to_string(firstPage))
            .append("-")
            .append(std::to_string(lastPage))
            .append(".");
    }
};

int main(int argc, char **argv)
{
    Publication pub;
    pub.setPrice(24);

    Book book(12);
    book.setAuthors("Manuel", "Oliveira", "Julia", "Roberts");
    book.setName("History of Cinematography");
    book.isbn = "9876543210";
    book.pages = 123;
    book.year = 1995;

    Article article(60);
    article.setAuthors("Joan", "Rider");
    article.setName("International Journal of Computation");
    article.issn = "1234567890";
    article.title = "A novel C++ program";
    article.year = 1996;
    article.volume = 4;
    article.issue = 2;
    article.firstPage = 45;
    article.lastPage = 99;

    Publication publications[20];
    publications[0] = pub;

    for (int i = 1; i < 20; i++)
    {
        if (i % 2 == 0)
        {
            publications[i] = book;
        }
        else
        {
            publications[i] = article;
        }
    }

    std::cout << "Prices:\n";

    for (Publication publication : publications)
    {
        std::cout << '$' << publication.getPrice() << std::endl;
    }

    std::cout << "\nCitation:\n"
              << article.citation() << std::endl;

    return 0;
}
