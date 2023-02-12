#include <ShaderGenerator/Symbols/ShaderVariable.hpp>

#include <sstream>

namespace sg
{
    ShaderVariable::ShaderVariable(const std::string& name, const std::string type, ShaderVariableQualifier qualifier, int location) :
        m_name(name),
        m_type(type),
        m_qualifier(qualifier),
        m_location(location)
    {

    }

    std::string ShaderVariable::generate() const
    {
        std::ostringstream stream;

        if (m_location > -1)
            stream << "layout(location = " << m_location << ") ";

        switch (m_qualifier)
        {
        case ShaderVariableQualifier::IN: stream << "in "; break;
        case ShaderVariableQualifier::OUT: stream << "out "; break;
        case ShaderVariableQualifier::UNIFORM: stream << "uniform "; break;
        }

        stream << m_type << " " << m_name << ";\n";

        return stream.str();
    }
}
