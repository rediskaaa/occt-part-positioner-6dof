#include <optional> 
#include <filesystem>
#include <TopoDS_Shape.hxx>

class StepImporter {
public:
	static std::optional<TopoDS_Shape> importFile(const std::filesystem::path& filePath);
};