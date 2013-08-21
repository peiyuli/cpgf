#ifndef __BUILDERCONTEXT_H
#define __BUILDERCONTEXT_H

#include "builderitem.h"
#include "buildersection.h"

#include "cpgf/gscopedptr.h"
#include "cpgf/gcallback.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <vector>
#include <deque>

namespace metagen {

class CppContext;
class CppContanier;
class CppFile;
class BuilderContext;
class BuilderFile;
class BuilderContainer;
class BuilderSection;
class BuilderSectionList;
class BuilderFileWriter;
class Project;

class BuilderContext
{
private:
	typedef std::vector<BuilderFileWriter *> BuilderFileWriterListType;

public:
	typedef std::vector<BuilderItem *> ItemListType;

public:
	BuilderContext(const Project * project, const std::string & sourceFileName);
	~BuilderContext();

	void process(const CppContext * cppContext);
	
	const Project * getProject() const { return this->project; }
	const std::string & getSourceBaseFileName() const { return this->sourceBaseFileName; }
	ItemListType * getItemList() { return &this->itemList; }
	BuilderSectionList * getSectionList() { return this->sectionList.get(); }

private:
	void doProcessFile(const CppFile * cppFile);
	void doPreocessByScript();
	void generateCodeSections();

	void generateCreationFunctionSections();
	void doGenerateCreateFunctionSection(BuilderSection * sampleSection,
		BuilderSectionListType::iterator begin, BuilderSectionListType::iterator end);

	void createHeaderFileWriter();
	void createSourceFileWriters();
	void doCollectPartialCreationFunctions(BuilderSectionListType * partialCreationSections);
	void doCreateSourceFileWriters(BuilderSectionListType * partialCreationSections);
	void doExtractPartialCreationFunctions(BuilderSectionListType * partialCreationSections,
		BuilderSectionListType * outputSections);

	void flatten(BuilderContainer * builderContainer);

	BuilderItem * createItem(const CppItem * cppItem);

private:
	const Project * project;
	std::string sourceFileName;
	std::string sourceBaseFileName;
	ItemListType itemList;
	cpgf::GScopedPointer<BuilderSectionList> sectionList;
	BuilderFileWriterListType fileWriterList;
	std::string creationFunctionNameCode;

	GMAKE_NONCOPYABLE(BuilderContext)
};


} // namespace metagen


#endif
