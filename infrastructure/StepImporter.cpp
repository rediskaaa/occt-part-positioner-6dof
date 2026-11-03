#include "StepImporter.hpp"
#include "STEPControl_Reader.hxx"

namespace {
	std::string pathToUtf8(const std::filesystem::path& path) {
		const std::u8string u8str = path.u8string();
		return std::string(reinterpret_cast<const char*>(u8str.data()), u8str.size());
	}
}

std::optional<TopoDS_Shape> StepImporter::importFile(const std::filesystem::path& filePath) {
	if (filePath.empty()) {
		std::cerr << "Error: empty file path";
		return std::nullopt;
	}
	try {
		const std::string utf8FilePath = pathToUtf8(filePath);
		STEPControl_Reader reader;
		IFSelect_ReturnStatus readStatus = reader.ReadFile(utf8FilePath.c_str());
		if (readStatus != IFSelect_RetDone) {
			std::cerr << "Error: Failed to read STEP file: " << filePath;
			return std::nullopt;
		}
		Standard_Integer nbRoots = reader.TransferRoots();
		if (nbRoots == 0) {
			std::cerr << "Error: No shapes transferred from STEP file:" << filePath;
			return std::nullopt;
		}
		TopoDS_Shape importedShape = reader.OneShape();
		if (importedShape.IsNull()) {
			std::cerr << "Error: No valid shape in STEP file:" << filePath;
			return std::nullopt;
		}

		return importedShape;
	} 
	catch (const Standard_Failure& e) {
		std::cerr << "OCCT error:" << e.GetMessageString();
	}
	catch (const std::exception& e) {
		std::cerr << "STL error:" << e.what();
	}
}