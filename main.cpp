#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <optional>
#include <ostream>
#include <regex>
#include <string>
#include <string_view>
#include <sstream>

constexpr char const* locale = "pt_PT.UTF-8";

template <class T>
std::optional<T> parse(std::string_view const value_string) noexcept
{
    std::regex const regex{R"r([1-9]\d{0,2}(\.\d{3})*(,\d+)?)r"};
    
    std::cmatch matches{};
    if (std::regex_match(value_string.data(), matches, regex))
    {
        std::string simplified_string_value = value_string.data();
        simplified_string_value.erase(
            std::remove(simplified_string_value.begin(), simplified_string_value.end(), '.'),
            simplified_string_value.end()
        );

        std::stringstream ss{simplified_string_value};
        ss.imbue(std::locale(locale));
        
        T value = 0.0;
        ss >> value;
        
        return value;
    }
    else
    {
        return {};
    }
}

template <class T>
T parse_line(std::istream& input_stream, std::ostream& output_stream) noexcept
{
    std::optional<T> result = {};

    while (!result.has_value())
    {
        std::string input_line = {};

        input_stream >> std::ws;
        std::getline(input_stream, input_line);

        result = parse<T>(input_line);

        if (!result.has_value())
        {
            output_stream << "Invalid value. Please try again: ";
        }
    }

    return *result;
}

template <class T>
T evaluate(T const pot_size, T const bet_size) noexcept
{
    return bet_size / (bet_size + pot_size);
}

template <class T>
void print_evaluate_calculation_description(std::ostream& output_stream, T const pot_size, T const bet_size) noexcept
{
    output_stream << "Calculation is:\n";
    output_stream << bet_size << " / " << "(" << pot_size << " + " << bet_size << ") = " << evaluate(pot_size, bet_size) << "\n";
}

void print_help(std::ostream& output_stream, std::string_view const program_name) noexcept
{
    output_stream << "Usage: " << program_name << " <pot_size> <bet_size>\n";
}

int main(int const argc, char const* const argv[]) noexcept
{
    std::cout << std::fixed << std::setprecision(3);

    std::string_view const program_name = argv[0];

    if (argc == 3)
    {
        std::ostream& output_stream = std::cout;
        
        std::optional<double> const pot_size = parse<double>(argv[1]);
        if (!pot_size.has_value())
        {
            print_help(output_stream, program_name);
            return -1;
        }

        std::optional<double> const bet_size = parse<double>(argv[2]);
        if (!bet_size.has_value())
        {
            print_help(output_stream, program_name);
            return -1;
        }
        double const percentage = evaluate<double>(*pot_size, *bet_size) * 100.0;
        output_stream << "You need > " << percentage << "% Equity.\n";

        print_evaluate_calculation_description(output_stream, *pot_size, *bet_size);
        output_stream << std::endl;
    }
    else
    {
        std::istream& input_stream = std::cin;
        std::ostream& output_stream = std::cout;

        output_stream << "Pot size: ";
        double const pot_size = parse_line<double>(input_stream, output_stream);

        output_stream << "Bet size: ";
        double const bet_size = parse_line<double>(input_stream, output_stream);

        double const percentage = evaluate(pot_size, bet_size) * 100.0;
        output_stream << "You need > " << percentage << "% Equity.\n";

        print_evaluate_calculation_description(output_stream, pot_size, bet_size);
        output_stream << std::endl;
    }
    
    return 0;
}