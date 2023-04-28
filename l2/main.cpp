#include <iostream>
#include <sstream>

class CustomClassThatCanConvertItselfToString
{
public:
    // Returns a string representation of the object
    std::string toString() const
    {
        return "IamBufferFromSstring";
    }

    // Overloads the stream insertion operator to output the object to an ostream
    friend std::ostream& operator<<(std::ostream& os, const CustomClassThatCanConvertItselfToString& obj)
    {
        os << obj.toString();
        return os;
    }
};

struct CustomStringBuffer
{
public:
    // Default constructor
    CustomStringBuffer() {};

    // Overloads the addition assignment operator to append the string representation of the given value to the buffer
    template<typename T>
    CustomStringBuffer& operator+=(const T& value)
    {
        std::stringstream ss;
        ss << value;
        buffer += getTypeString(value);
        buffer += ":";

        // If T is bool, append "true" if value is true, else append "false"
        if constexpr (std::is_same_v<T, bool>) {
            buffer += (value) ? "true" : "false";
        }
        // Otherwise, append type string and value string to buffer
        else {
            buffer += ss.str();
        }

        buffer += " ";
        return *this;
    }

    // Overloads the stream insertion operator to output an object to an ostream using its toString() method
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const T& obj)
    {
        os << obj.toString();
        return os;
    }

    // Overloads the stream insertion operator to output the buffer to an ostream
    friend std::ostream& operator<<(std::ostream& os, const CustomStringBuffer& buffer)
    {
        os << buffer.buffer;
        return os;
    }

private:
    // The buffer containing the string representation of the appended values
    std::string buffer = "";

    // Returns a string representing the type of the given value (used for outputting the buffer)
    template<typename T>
    std::string getTypeString(const T&) const
    {
        return "CTS";  // Convert To String template overload
    }

    std::string getTypeString(const int&) const
    {
        return "Ar";  // Arithmetic template overload
    }

    std::string getTypeString(const long long&) const
    {
        return "Ar";  // Arithmetic template overload
    }

    std::string getTypeString(const double&) const
    {
        return "Ar";  // Arithmetic template overload
    }

    std::string getTypeString(const float&) const
    {
        return "Ar";  // Arithmetic template overload
    }

    std::string getTypeString(const bool&) const
    {
        return "B";  // Boolean template overload
    }

    std::string getTypeString(const CustomClassThatCanConvertItselfToString&) const
    {
        return "CTS";  // Convert To String template overload
    }
};


int main()
{
    CustomClassThatCanConvertItselfToString Sstring;
    std::string string = "string_first";

    CustomStringBuffer SBuffer;  // buffer = ""
    SBuffer += string;           // buffer = "CTS:string_first"
    SBuffer += 42;               // buffer = "CTS:string_first Ar:42"
    SBuffer += 4.2;              // buffer = "CTS:string_first Ar:42 Ar:4.2"
    SBuffer += -42424224244242;  // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242"
    SBuffer += Sstring;          // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242 CTS:IamBufferFromSstring"
    SBuffer += false;            // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242 CTS:IamBufferFromSstring B:false"
    
    std::cout << SBuffer << std::endl;

    // CTS - Convert To String template overload
    // Ar  - Arithmetic template overload
    // B   - Boolean template overload

    return 0;
}
