#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class BaseStructure {
public:
    virtual void visualize() const = 0;
    virtual ~BaseStructure() {}
};

class Vector : public BaseStructure {
private:
    double* data;
    size_t size;

    class Proxy {
        Vector& v;
        size_t idx;
    public:
        Proxy(Vector& vec, size_t i) : v(vec), idx(i) {}
        Proxy& operator=(double val) {
            if (idx < v.size) v.data[idx] = val;
            return *this;
        }
        operator double() const { return v.data[idx]; }
    };

public:
    Vector() : data(nullptr), size(0) {}
    Vector(size_t s, double val = 0.0) : size(s) {
        data = new double[size];
        for (size_t i = 0; i < size; ++i) data[i] = val;
    }
    Vector(const Vector& other) : size(other.size) {
        data = new double[size];
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
    }
    ~Vector() { delete[] data; }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new double[size];
            for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
        }
        return *this;
    }
    Proxy operator[](size_t i) { return Proxy(*this, i); }
    double operator[](size_t i) const { return data[i]; }
    Vector* operator->() { return this; }

    void visualize() const override {
        std::cout << "[ ";
        for (size_t i = 0; i < size; ++i) std::cout << data[i] << " ";
        std::cout << "]\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << v.size << " ";
        for (size_t i = 0; i < v.size; ++i) os << v.data[i] << " ";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vector& v) {
        size_t s;
        if (is >> s) {
            delete[] v.data;
            v.size = s;
            v.data = new double[s];
            for (size_t i = 0; i < s; ++i) is >> v.data[i];
        }
        return is;
    }

    double length() const {
        double sum = 0;
        for (size_t i = 0; i < size; ++i) sum += data[i] * data[i];
        return std::sqrt(sum);
    }
    void sort() {
        for (size_t i = 0; i < size; ++i)
            for (size_t j = i + 1; j < size; ++j)
                if (data[i] > data[j]) std::swap(data[i], data[j]);
    }
};

int main() {
    std::vector<Vector> list;
    int choice;

    do {
        std::cout << "\n1. Add vector\n2. Show all\n3. Save to file\n4. Load from file\n5. Sort first vector\n0. Exit\nChoice: ";
        std::cin >> choice;

        if (choice == 1) {
            size_t s;
            std::cout << "Size: "; std::cin >> s;
            Vector v(s);
            std::cout << "Elements: ";
            for (size_t i = 0; i < s; ++i) {
                double val; std::cin >> val;
                v[i] = val;
            }
            list.push_back(v);
        } else if (choice == 2) {
            for (size_t i = 0; i < list.size(); ++i) {
                std::cout << "Vector " << i << ": ";
                list[i]->visualize();
            }
        } else if (choice == 3 && !list.empty()) {
            std::ofstream out("vector.txt");
            out << list[0];
            out.close();
            std::cout << "First vector saved to vector.txt!\n";
        } else if (choice == 4) {
            std::ifstream in("vector.txt");
            Vector v;
            in >> v;
            in.close();
            list.push_back(v);
            std::cout << "Loaded from file!\n";
        } else if (choice == 5 && !list.empty()) {
            list[0].sort();
            std::cout << "Sorted!\n";
        }
    } while (choice != 0);

    return 0;
}
