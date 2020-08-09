#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include "Matrix.hpp"
#include <cmath>
#include <vector>
#include <fstream>
#include <stdexcept>

// function implementation from https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
static inline unsigned long fastRand()
{
    static unsigned long x=123456789, y=362436069, z=521288629;
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}
//

template<size_t N, size_t N2>
static void randomizeMatrix(float chance, std::pair<float, float> interval, Matrix<float, N, N2>* matrix)
{
    constexpr int precision = 1048576;
    for (size_t i = 0; i < N2; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            if (fastRand()%precision <= chance*precision)
            {
                float modification = interval.first + (interval.second - interval.first)*
                                                      (static_cast<float>(fastRand()%precision)/precision);
                (*matrix)[i][j] += modification;
            }
        }
    }
}

template <size_t N, size_t N2, size_t... args>
class NeuralNetwork
{
public:

    NeuralNetwork() {}

    decltype(auto) feedForward(Matrix<float, N, 1>& vector)
    {
        return feedForwardRec<N, N2, args...>(vector, _matrices);
    }
    void mutate(float chance, std::pair<float, float> interval)
    {
        mutateRec(chance, interval, &_matrices);
    }
    void saveToFile(const std::string& fileName) const
    {
        std::ofstream myFile;
        myFile.open (fileName);
        if (!myFile.good())
            throw std::invalid_argument("Cannot open file: " + fileName);
        myFile << *this;
        myFile.close();
    }

    void loadFromFile(const std::string& fileName)
    {
        std::ifstream myFile;
        myFile.open (fileName);
        if (!myFile.good())
            throw std::invalid_argument("Cannot open file: " + fileName);
        myFile >> *this;
        myFile.close();
    }
    template<size_t NN, size_t NN2, size_t... Nargs>
    friend std::ostream& operator<<(std::ostream &os, const NeuralNetwork<NN, NN2, Nargs...>& network);
    template<size_t NN, size_t NN2, size_t... Nargs>
    friend std::istream& operator>>(std::istream &os, NeuralNetwork<NN, NN2, Nargs...>& network);

private:
    template <size_t NN, size_t NN2, size_t... Nargs>
    struct Node
    {
        Node() : matrix(0) {}
        Matrix<float, NN2, NN> matrix;
        Node<NN2, Nargs...> next;
    };
    template <size_t NN, size_t NN2>
    struct Node<NN, NN2>
    {
        Node() : matrix(0) {}
        Matrix<float, NN2, NN> matrix;
    };

    template <size_t NN, size_t NN2, size_t... Nargs>
    decltype(auto) feedForwardRec(Matrix<float, NN, 1>& vector, const Node<NN, NN2, Nargs...>& matrix)
    {
        Matrix<float, NN2, 1> newVec = vector*matrix.matrix;
        for (size_t i = 0; i < NN2; i++)
            newVec[0][i] = std::max(-1.f, newVec[0][i]);
        return feedForwardRec<NN2, Nargs...>(newVec, matrix.next);
    }
    template <size_t NN, size_t NN2>
    decltype(auto) feedForwardRec(Matrix<float, NN, 1>& vector, const Node<NN, NN2>& matrix)
    {
        Matrix<float, NN2, 1> newVec = vector*matrix.matrix;
        for (size_t i = 0; i < NN2; i++)
            newVec[0][i] = std::tanh(newVec[0][i]);
        return newVec;
    }
    template <size_t NN, size_t NN2, size_t... Nargs>
    void mutateRec(float chance, std::pair<float, float> interval, Node<NN, NN2, Nargs...>* matrix)
    {
        randomizeMatrix(chance, interval, &matrix->matrix);
        mutateRec(chance, interval, &matrix->next);
    }
    template <size_t NN, size_t NN2>
    void mutateRec(float chance, std::pair<float, float> interval, Node<NN, NN2>* matrix)
    {
        randomizeMatrix(chance, interval, &matrix->matrix);
    }
    template <size_t NN, size_t NN2, size_t... Nargs>
    void ostreamOperatorRec(std::ostream& os, const Node<NN, NN2, Nargs...>* matrix) const
    {
        os<<matrix->matrix<<std::endl;
        ostreamOperatorRec(os, &matrix->next);
    }
    template <size_t NN, size_t NN2>
    void ostreamOperatorRec(std::ostream& os, const Node<NN, NN2>* matrix) const
    {
        os<<matrix->matrix;
    }
    template <size_t NN, size_t NN2, size_t... Nargs>
    void istreamOperatorRec(std::istream& is, Node<NN, NN2, Nargs...>* matrix)
    {
        is>>matrix->matrix;
        istreamOperatorRec(is, &matrix->next);
    }
    template <size_t NN, size_t NN2>
    void istreamOperatorRec(std::istream& is, Node<NN, NN2>* matrix)
    {
        is>>matrix->matrix;
    }

    Node<N, N2, args...> _matrices;
};

template <size_t N, size_t N2, size_t... args>
std::ostream& operator<<(std::ostream& os, const NeuralNetwork<N, N2, args...>& network)
{
    network.ostreamOperatorRec(os, &network._matrices);
    return os;
}

template <size_t N, size_t N2, size_t... args>
std::istream& operator>>(std::istream& is, NeuralNetwork<N, N2, args...>& network)
{
    network.istreamOperatorRec(is, &network._matrices);
    return is;
}

#endif // NEURALNETWORK_HPP
