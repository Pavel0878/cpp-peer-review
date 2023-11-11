#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain {
public:
    // разработайте класс домена
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(const string& domain) 
       : domain_(domain.crbegin(), domain.crend())
    {
        domain_.push_back('.');
    }
    // разработайте operator==
    bool operator == (const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator < (const Domain& other) const {
        return lexicographical_compare(domain_.begin(), domain_.end(), other.domain_.begin(), other.domain_.end());
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& other) const {
        if (other.domain_.size() > domain_.size()) {
            return false;
        }
        return equal(other.domain_.begin(), other.domain_.end(), domain_.begin());
    }

private:
    string domain_;
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template <typename RandomIt>
    DomainChecker(RandomIt first, RandomIt last) 
        :forbidden_domains_(first,last)
    {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto it = unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {return rhs.IsSubdomain(lhs); });
        forbidden_domains_.erase(it, forbidden_domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain) {
        if (forbidden_domains_.empty()) { return false; }
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it != forbidden_domains_.begin()) {
            return domain.IsSubdomain(*(it - 1));
        }
        else {
            return domain.IsSubdomain(*it);
        }
    }

private:
    vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
vector<Domain> ReadDomains(istream& input, Number number) {
    vector<Domain> domains;
    for (Number num = 0; num < number; ++num) {
        string line;
        getline(input, line);
        domains.emplace_back(line);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

void TestIsSubdomain() {
    Domain doc("aa.a");
    Domain a("ab.aa.a");
    Domain b("aa.a");
    Domain c("a");
    assert(a.IsSubdomain(doc));
    assert(b.IsSubdomain(doc));
    assert(!c.IsSubdomain(doc));
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}