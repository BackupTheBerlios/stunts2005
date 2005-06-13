#include<iostream>
#include<string>

using namespace std;

int main()
{
	//print welcome message
	cout << "convert - trk to xml" << endl << endl;


	//open files
	FILE* templateFile;
	FILE* trackFile;

	templateFile = fopen("track object template.xml", "rb");
	trackFile = fopen("track file.trk", "rb");

	if (!templateFile || !trackFile)
	{
		cout << "error opening files" << endl;
		return 1;
	}
	cout << "convert: files are open" << endl << endl;


	//get file length
	long templateFileLength, trackFileLength;

	fseek(templateFile, 0, SEEK_END);
	fseek(trackFile, 0, SEEK_END);

	templateFileLength = ftell(templateFile);
	trackFileLength = ftell(trackFile);

	fseek(templateFile, 0, SEEK_SET);
	fseek(trackFile, 0, SEEK_SET);

	cout << "templateFileLength: " << templateFileLength << endl;
	cout << "trackFile: " << trackFileLength << endl << endl;


	//read files
	char* templateChars = new char[templateFileLength];
	char* trackChars = new char[trackFileLength];

	int bytesRead;

	try
	{
		bytesRead = fread(templateChars, 1, templateFileLength, templateFile);
		if (bytesRead!=templateFileLength)
		{
			cout << "error reading templateFile" << endl;
			throw 1;
		}

		fread(trackChars, 1, trackFileLength, trackFile);
		if (bytesRead!=templateFileLength)
		{
			cout << "error reading templateFile" << endl;
			throw 1;
		}
	}
	catch(int errNo)
	{
		if (errNo == 1)
		{
			if (templateChars)
				delete templateChars;
			if (trackChars)
				delete trackChars;

			return 1;
		}
	}

	//close files
	fclose(templateFile);
	fclose(trackFile);
	cout << "convert: files closed" << endl << endl;


	//create strins from templateChars
	string templateString(templateChars, templateFileLength);


	//delete templateChars
	if (templateChars)
		delete templateChars;


	//read formatted string
	char* currentTrackChars = trackChars;

	char* posX = new char[16];			memset(posX, 0, 16);
	char* posZ = new char[16];			memset(posZ, 0, 16);
	char* rotY = new char[16];			memset(rotY, 0, 16);
	char* scaleA = new char[16];		memset(scaleA, 0, 16);
	char* meshName = new char[260];	memset(meshName, 0, 260);

	string currentTemplateString;
	string totalTrackString;

	//begin root tag
	totalTrackString += "<track>\n\t<objects>\n";


	//ignore first line
	currentTrackChars = strchr(currentTrackChars, 10) + 1;

	//scan the whole file
	while(currentTrackChars != NULL)
	{
		//ignore empty lines
		while ((currentTrackChars != NULL) && (currentTrackChars[0]==10))
		{
			currentTrackChars = strchr(currentTrackChars, 10);
			if (currentTrackChars != NULL)
				currentTrackChars++;
		}
		if (!currentTrackChars || !currentTrackChars[0])
			break;

		//scan line
		sscanf(currentTrackChars, "%s%s%s%s%s", posX, posZ, rotY, scaleA, meshName);

		//one line down
		currentTrackChars = strchr(currentTrackChars, 10);

		//modify template
		currentTemplateString.assign(templateString);

		const char* _OBJECT_COMMENT_ = "_OBJECT_COMMENT_";
		const char* _ROT_Y_ = "_ROT_Y_";
		const char* _POS_X_ = "_POS_X_";
		const char* _POS_Z_ = "_POS_Z_";
		const char* _SCALE_AXIS_ = "_SCALE_AXIS_";
		const char* _MESH_NAME_ = "_MESH_NAME_";

		currentTemplateString.replace(
			currentTemplateString.find(_OBJECT_COMMENT_, 0),
			strlen(_OBJECT_COMMENT_), meshName);
		currentTemplateString.replace(
			currentTemplateString.find(_ROT_Y_, 0),
			strlen(_ROT_Y_), rotY);
		currentTemplateString.replace(
			currentTemplateString.find(_POS_X_, 0),
			strlen(_POS_X_), posX);
		currentTemplateString.replace(
			currentTemplateString.find(_POS_Z_, 0),
			strlen(_POS_Z_), posZ);
		currentTemplateString.replace(
			currentTemplateString.find(_SCALE_AXIS_, 0),
			strlen(_SCALE_AXIS_), scaleA);
		currentTemplateString.replace(
			currentTemplateString.find(_MESH_NAME_, 0),
			strlen(_MESH_NAME_), meshName);

		//append this template
		totalTrackString+=currentTemplateString + "\n" + "\n";

		//write status message
		cout << "convert: processing " << meshName <<
			" at (" << posX << "," << posZ << ")" << endl;
	}

	//delete old buffers
	if (meshName)
		delete meshName;

	if (trackChars)
		delete trackChars;

	//close root tag
	totalTrackString += "\t</objects>\n</track>";

	//write file
	FILE* totalTrackFile;

	totalTrackFile = fopen("total_track_objects.xml", "wb");

	if (!totalTrackFile)
	{
		cout << endl << "error opening totalTrackFile" << endl;
		return 1;
	}
	cout << endl << "convert: totalTrackFile is open" << endl;

	int numBytes;
	numBytes = fwrite(totalTrackString.c_str(), 1, totalTrackString.length(),
					totalTrackFile);
	cout << "convert: totalTrackFile file written (" <<
		numBytes << " bytes)" << endl;

	fclose(totalTrackFile);
	cout << "convert: totalTrackFile closed" << endl << endl;

	//exit
	cout << "convert: exit application" << endl;
	return 0;
}
