#include <xlnt/serialization/shared_strings_serializer.hpp>
#include <xlnt/serialization/xml_document.hpp>
#include <xlnt/serialization/xml_node.hpp>

namespace xlnt {

xml_document shared_strings_serializer::write_shared_strings(const std::vector<string> &strings)
{
    xml_document xml;

    auto root_node = xml.add_child("sst");

    xml.add_namespace("", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");

    root_node.add_attribute("uniqueCount", string::from(strings.size()));

    for (const auto &string : strings)
    {
        root_node.add_child("si").add_child("t").set_text(string);
    }

    return xml;
}

bool shared_strings_serializer::read_shared_strings(const xml_document &xml, std::vector<string> &strings)
{
    strings.clear();

    auto root_node = xml.get_child("sst");

    auto unique_count = root_node.get_attribute("uniqueCount").to<std::size_t>();

    for (const auto &si_node : root_node.get_children())
    {
        if (si_node.get_name() != "si")
        {
            continue;
        }

        if (si_node.has_child("t"))
        {
            strings.push_back(si_node.get_child("t").get_text());
        }
        else if (si_node.has_child("r"))
        {
            strings.push_back(si_node.get_child("r").get_child("t").get_text());
        }
    }

    if (unique_count != strings.size())
    {
        throw std::runtime_error("counts don't match");
    }

    return true;
}

} // namespace xlnt