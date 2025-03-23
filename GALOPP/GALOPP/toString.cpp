module toString;

std::string complexToString(const std::complex<double>& value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << value.real() << (value.imag() < 0 ? " - j" : " + j") << std::abs(value.imag());
    return oss.str();
}

std::string doubleToString(const double& value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << value;
    return oss.str();
}