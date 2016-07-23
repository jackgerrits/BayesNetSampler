// Vector printing function that overloads the stream operator. Useful for debugging.
template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    out << "{";
    for(int i = 0; i < v.size(); i++) {
        out << v[i];
        if(i != (v.size()-1)) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}

// Applies a mapping operation over each element of the vector and returns
// another vector holding the result of these operations.
// Accepts a vector holding abritrary values and a lambda function that accepts
// the given vectors type and returns an abritrary type.
template <typename S, typename T>
std::vector<S> vectorMap(const std::vector<T>& container, const std::function <S (T)>& func) {
    // Create a new vector of the return type.
    std::vector<S> resultVector;
    // Iterate over each element in the provided vector and push the result of of using the given func onto the result vector.
    for (T element : container) {
        resultVector.push_back(func(element));
    }
    return resultVector;
}
