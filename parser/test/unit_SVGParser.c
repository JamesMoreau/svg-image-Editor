/**
 * James Moreau
 * ID: 1065510
 * E: jmorea03@uoguelph.ca
 */

#include "SVGParser.h"
#include "Helpers.h"
#include <libxml/tree.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int createSVGimage_test1(void) {

	SVGimage* temp = NULL;
	char* f = "../test/Emoji_poo.svg";

	printf("Opening: %s\n", f);
	temp = createSVGimage(f);

	if(!temp) {
		printf(ANSI_COLOR_RED   "createSVGimage_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	if(strcmp(temp->namespace, "http://www.w3.org/2000/svg") == 0 ) {
		printf(ANSI_COLOR_GREEN   "createSVGimage_test1"   ANSI_COLOR_RESET "\n");
		printf("\tfirst namespace: %s\n", temp->namespace);
		deleteSVGimage(temp);
		return (1);
	} else {
		printf(ANSI_COLOR_RED   "createSVGimage_test1" ANSI_COLOR_RESET"\n");
		deleteSVGimage(temp);
		return (0);
	}
}

int createSVGimage_test2(void) {

	SVGimage* temp = NULL;
	char* f = "badFileName.svg";

	printf("Opening: %s\n", f);
	temp = createSVGimage(f);

	if(!temp) {
		printf(ANSI_COLOR_GREEN   "createSVGimage_test2"   ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return (1);
	} else {
		printf(ANSI_COLOR_RED   "createSVGimage_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return (0);
	}
}

int createSVGimage_test3(void) {

	SVGimage* temp = NULL;
	char* f = "../test/accessible.svg";

	printf("Opening: %s\n", f);
	temp = createSVGimage(f);

	if(!temp) {
		printf(ANSI_COLOR_RED   "createSVGimage_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	if (strcmp(temp->title, "accessible")) {
		printf(ANSI_COLOR_RED   "createSVGimage_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	printf(ANSI_COLOR_GREEN   "createSVGimage_test3"   ANSI_COLOR_RESET "\n");
	deleteSVGimage(temp);
	return (1);
}

int createSVGimage_test4(void) {

	SVGimage* temp = NULL;
	char* f = "../test/rect.svg";

	printf("Opening: %s\n", f);
	temp = createSVGimage(f);

	if(!temp) {
		printf(ANSI_COLOR_RED   "createSVGimage_test4" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	if (strcmp(temp->title, "Example quad01 - quadratic Bézier commands in path data")) {
		printf(ANSI_COLOR_RED   "createSVGimage_test4" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	if (strcmp(temp->description,
	"Picture showing a \"Q\" a \"T\" command, along with annotations showing the control points and end points"
	)) {
		printf(ANSI_COLOR_RED   "createSVGimage_test4" ANSI_COLOR_RESET "\n");
		deleteSVGimage(temp);
		return(0);
	}

	printf(ANSI_COLOR_GREEN   "createSVGimage_test4"   ANSI_COLOR_RESET "\n");
	deleteSVGimage(temp);
	return (1);
}

int SVGimage_Constructor_test1(void) {
	SVGimage* img = NULL;

	img = SVGimage_Constructor();

	if (
		(!img->circles) ||
		(!img->rectangles ) ||
		(!img->paths ) ||
		(!img->groups) ||
		(!img->otherAttributes)
	) {
		printf(ANSI_COLOR_RED   "SVGimage_Constructor_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(img);
		return (0);
	}

	printf("intialized lists:\n%p\n%p\n%p\n%p\n%p\n", img->circles,
												img->rectangles,
												img->paths,
												img->groups,
												img->otherAttributes);

	printf(ANSI_COLOR_GREEN   "SVGimage_Constructor_test1"   ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	return (1);
}

int deleteSVGimage_test1(void) {

	SVGimage* img = NULL;
	char* f = "../test/bzrfeed.svg";

	printf("Opening: %s\n", f);
	img = createSVGimage(f);

	deleteSVGimage(img);

	printf(ANSI_COLOR_GREEN   "deleteSVGimage_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int SVGimageToString_test1(void) {

	SVGimage* img = NULL;
	char* s  = NULL;
	char* f = "../test/rec_with_units.svg";

	printf("Opening: %s\n", f);
	img = createSVGimage(f);

	s = SVGimageToString(img);
	if (!s) {
		printf(ANSI_COLOR_RED   "SVGimageToString_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(img);
		return (0);
	}

	printf("%s", s);
	deleteSVGimage(img);
	free(s);
	printf(ANSI_COLOR_GREEN   "SVGimageToString_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int SVGimageToString_test2(void) {

	SVGimage* img = NULL;
	char* s = NULL;
	char* f = "../test/Emoji_poo.svg";

	printf("Opening: %s\n", f);
	img = createSVGimage(f);

	s = SVGimageToString(img);
	if (!s) {
		printf(ANSI_COLOR_RED   "SVGimageToString_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(img);
		return (0);
	}

	printf("%s", s);
	deleteSVGimage(img);
	free(s);
	printf(ANSI_COLOR_GREEN   "SVGimageToString_test2" ANSI_COLOR_RESET "\n");
	return (1);
}

int SVGimageToString_test3(void) {

	SVGimage* img = NULL;
	char* s = NULL;
	char* f = "../test/bzrfeed.svg";

	printf("Opening: %s\n", f);
	img = createSVGimage(f);

	s = SVGimageToString(img);
	if (!s) {
		printf(ANSI_COLOR_RED   "SVGimageToString_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(img);
		return (0);
	}

	printf("%s", s);
	deleteSVGimage(img);
	free(s);
	printf(ANSI_COLOR_GREEN   "SVGimageToString_test3" ANSI_COLOR_RESET "\n");
	return (1);
}

int SVGimageToString_test4(void) {
	SVGimage* img = NULL;
	char* s = NULL;
	char* f = "../test/quad01.svg";

	printf("Opening: %s\n", f);
	img = createSVGimage(f);

	s = SVGimageToString(img);
	if (!s) {
		printf(ANSI_COLOR_RED   "SVGimageToString_test4" ANSI_COLOR_RESET "\n");
		deleteSVGimage(img);
		return (0);
	}

	printf("%s", s);
	deleteSVGimage(img);
	free(s);
	printf(ANSI_COLOR_GREEN   "SVGimageToString_test4" ANSI_COLOR_RESET "\n");
	return (1);
}

int Attribute_Constructor_test1(void) {
	char* n = "TestName";
	char* v = "TestValue Cm";

	Attribute* a = Attribute_Constructor(n, v);

	if(!a) {
		printf(ANSI_COLOR_RED   "Attribute_Constructor_test1" ANSI_COLOR_RESET "\n");
		deleteAttribute((void*) a);
		return (0);
	}

	printf(ANSI_COLOR_GREEN  "Attribute_Constructor_test1" ANSI_COLOR_RESET "\n");
	deleteAttribute((void*) a);
	return (1);
}

int Attribute_Constructor_test2(void) {
	Attribute* a = Attribute_Constructor(NULL, NULL);

	if(a) {
		printf(ANSI_COLOR_RED  "Attribute_Constructor_test2" ANSI_COLOR_RESET "\n");
		deleteAttribute((void*) a);
		return (0);
	}

	printf(ANSI_COLOR_GREEN   "Attribute_Constructor_test2" ANSI_COLOR_RESET "\n");
	deleteAttribute((void*) a);
	return (1);
}

int Rectangle_Contructor_test1() {
	float x = 1.23;
	float y = 3.21;
	float w = 5;
	float h = 10.5;
	char* u = "Cm";

	Rectangle* rec = NULL;

	rec = Rectangle_Contructor();
	rec->x = x;
	rec->y = y;
	rec->height = h;
	rec->width = w;
	strcpy(rec->units, u);


	if(!rec) {
		printf(ANSI_COLOR_RED  "Rectangle_Contructor_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (
		(rec->x != x) ||
		(rec->y != y) ||
		(rec->width != w) ||
		(rec->height != h)
	) {
		printf(ANSI_COLOR_RED  "Rectangle_Contructor_test1" ANSI_COLOR_RESET "\n");
		deleteRectangle(rec);
		return (0);
	}

	if (!rec->otherAttributes) {
		printf(ANSI_COLOR_RED  "Rectangle_Contructor_test1" ANSI_COLOR_RESET "\n");
		free(rec);
		return (0);
	}

	printf(ANSI_COLOR_GREEN  "Rectangle_Contructor_test1" ANSI_COLOR_RESET "\n");
	deleteRectangle(rec);
	return (1);
}

int rectangleToString_test1() {
	float x = 16;
	float y = 12;
	float w = 30.0;
	float h = 10.5;
	char* u = "Cm";

	Rectangle* rec = NULL;
	rec = Rectangle_Contructor();
	rec->x = x;
	rec->y = y;
	rec->height = h;
	rec->width = w;
	strcpy(rec->units, u);

	char* s = NULL;
	s = rectangleToString((void*) rec);

	if (!s) {
		printf(ANSI_COLOR_RED  "rectangleToString_test1" ANSI_COLOR_RESET "\n");
		deleteRectangle(rec);
		return (0);
	}

	printf("%s\n", s);
	printf(ANSI_COLOR_GREEN  "rectangleToString_test1" ANSI_COLOR_RESET "\n");
	deleteRectangle(rec);
	free(s);
	return (1);
}

int compareRectangles_test1() {
	float x = 16;
	float y = 12;
	float w = 30.0;
	float h = 10.5;
	char* u = "Cm";

	Rectangle* r1 = NULL;
	Rectangle* r2 = NULL;

	r1 = Rectangle_Contructor();
	r1->x = x;
	r1->y = y;
	r1->height = h;
	r1->width = w;
	strcpy(r1->units, u);

	r2 = Rectangle_Contructor();
	r2->x = x;
	r2->y = y;
	r2->height = h;
	r2->width = w;
	strcpy(r2->units, u);

	printf("%p\n", r1);
	printf("%p\n", r2);

	if ((!r1) || (!r2)) {
		printf(ANSI_COLOR_RED  "compareRectangles_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (compareRectangles(r1, r2) != 1) {
		printf(ANSI_COLOR_RED  "compareRectangles_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN  "compareRectangles_test1" ANSI_COLOR_RESET "\n");
	deleteRectangle(r1);
	deleteRectangle(r2);
	return (1);
}

int Circle_Constructor_test1() {
	float cx = 1.23;
	float cy = 65.4;
	float r = 89.321;
	char* u = "Feet";

	Circle* c = Circle_Constructor();

	if(!c) {
		printf(ANSI_COLOR_RED  "Circle_Constructor_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	c->cx = cx;
	c->cy = cy;
	c->r = r;
	strcpy(c->units, u);

	if (!compareCircles(c, c)) {
		printf(ANSI_COLOR_RED  "Circle_Constructor_test1" ANSI_COLOR_RESET "\n");
		deleteCircle(c);
		return (0);
	}

	printf(ANSI_COLOR_GREEN  "Circle_Constructor_test1" ANSI_COLOR_RESET "\n");
	deleteCircle(c);
	return (1);
}

int circleToString_test1() {
	float cx = 1.23;
	float cy = 65.4;
	float r = 89.321;
	char* u = "Feet";

	Circle* c = Circle_Constructor();
	if(!c) {
		printf(ANSI_COLOR_RED  "circleToString_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	c->cx = cx;
	c->cy = cy;
	c->r = r;
	strcpy(c->units, u);

	char* str = NULL;
	str = circleToString(c);
	if(!str) {
		printf(ANSI_COLOR_RED  "circleToString_test1" ANSI_COLOR_RESET "\n");
		deleteCircle(c);
		return (0);
	}

	printf("%s\n", str);
	free(str);
	deleteCircle(c);
	printf(ANSI_COLOR_GREEN "circleToString_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int pathToString_test1() {
	Path* p = NULL;

	char* d = "m38 50.6c0 3.3-2.7 6-6 6-3.3 0-6-2.7-6-6 0-3.3 2.7-6 6-6 3.3 0 6 2.7 6 6";

	p = Path_Constructor();
	p->data = malloc(sizeof(char) * (strlen(d) + 1));
	strcpy(p->data, d);

	if(!p) {
		printf(ANSI_COLOR_RED "pathToString_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* s;
	s = pathToString(p);
	if(!s) {
		printf(ANSI_COLOR_RED "pathToString_test1" ANSI_COLOR_RESET "\n");
		deletePath(p);
		return (0);
	}

	printf("%s\n", s);
	deletePath(p);
	free(s);
	printf(ANSI_COLOR_GREEN "pathToString_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int Group_Constructor_test1() {
	Group* g = Group_Constructor();

	if(!g) {
		printf(ANSI_COLOR_RED "Group_Constructor_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	deleteGroup(g);
	printf(ANSI_COLOR_GREEN "Group_Constructor_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int getRects_test1() {
	SVGimage* image = createSVGimage("../test/Brutal3.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "getRects_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	List* list = getRects(image);
	if(!list) {
		printf(ANSI_COLOR_RED "getRects_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	char* string = toString(list);
	if(!string) {
		printf(ANSI_COLOR_RED "getRects_test1" ANSI_COLOR_RESET "\n");
		freeList(list);
		deleteSVGimage(image);
		return (0);
	}

	printf("%s\n", string);

	printf(ANSI_COLOR_GREEN "getRects_test1" ANSI_COLOR_RESET "\n");
	free(string);
	freeList(list);
	deleteSVGimage(image);
	return (1);
}

int getCircles_test1() {
	SVGimage* image = createSVGimage("../test/bzrfeed.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "getCircles_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	List* list = getCircles(image);
	if(!list) {
		printf(ANSI_COLOR_RED "getCircles_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* string = toString(list);
	if(!string) {
		printf(ANSI_COLOR_RED "getCircles_test1" ANSI_COLOR_RESET "\n");
		freeList(list);
		deleteSVGimage(image);
		return (0);
	}

	printf("%s\n", string);
	printf(ANSI_COLOR_GREEN "getCircles_test1" ANSI_COLOR_RESET "\n");
	free(string);
	freeList(list);
	deleteSVGimage(image);
	return (1);
}

int getPaths_test1() {
	SVGimage* image = createSVGimage("../test/bzrfeed.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "getPaths_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	List* list = getPaths(image);
	if(!list) {
		printf(ANSI_COLOR_RED "getPaths_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	char* string = toString(list);
	if(!string) {
		printf(ANSI_COLOR_RED "getPaths_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf("%s\n", string);
	printf(ANSI_COLOR_GREEN "getPaths_test1" ANSI_COLOR_RESET "\n");
	free(string);
	freeList(list);
	deleteSVGimage(image);
	return (1);
}

int getGroups_test1() {
	SVGimage* image = createSVGimage("../test/bzrfeed.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "getGroups_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	List* list = getGroups(image);
	if(!list) {
		printf(ANSI_COLOR_RED "getGroups_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	char* string = toString(list);
	if(!string) {
		printf(ANSI_COLOR_RED "getGroups_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf("%s\n", string);
	printf(ANSI_COLOR_GREEN "getGroups_test1" ANSI_COLOR_RESET "\n");
	free(string);
	freeList(list);
	deleteSVGimage(image);
	return (1);
}

int numRectsWithArea_test1() {
	SVGimage* image = createSVGimage("../test/Brutal3.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numRectsWithArea_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (numRectsWithArea(image, 716404) != 1) {
		printf(ANSI_COLOR_RED "numRectsWithArea_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numRectsWithArea_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int numCirclesWithArea_test1() {
	SVGimage* image = createSVGimage("../test/bzrfeed.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numCirclesWithArea_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf("num:%d\n", numCirclesWithArea(image, (9)*(9)*(3.1415926535)));

	if ( numCirclesWithArea(image, (9)*(9)*(3.1415926535) ) != 3 ) {
		printf(ANSI_COLOR_RED "numCirclesWithArea_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numCirclesWithArea_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int numPathsWithdata_test1() {
	SVGimage* image = createSVGimage("../test/accessible.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numPathsWithdata_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* d = "M8,3h84c3,0,5,2,5,5v84c0,3-2,5-5,5h-84c-3,0-5-2-5-5v-84c0-3,2-5,5-5";
	if (numPathsWithdata(image, d) != 1) {
		printf(ANSI_COLOR_RED "numPathsWithdata_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numPathsWithdata_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int numGroupsWithLen_test1() {
	SVGimage* image = createSVGimage("../test/Emoji_poo.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numGroupsWithLen_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (numGroupsWithLen(image, 2) != 1) {
		printf(ANSI_COLOR_RED "numGroupsWithLen_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numGroupsWithLen_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int numAttr_test1() {
	SVGimage* image = createSVGimage("../test/Emoji_poo.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numAttr_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf("count: %d\n", numAttr(image));
	if(numAttr(image) != 9) {
		printf(ANSI_COLOR_RED "numAttr_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numAttr_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int numAttr_test2() {
	SVGimage* image = createSVGimage("../test/quad01.svg");
	if (!image) {
		printf(ANSI_COLOR_RED "numAttr_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	/* printf("count: %d\n", numAttr(image)); */
	if(numAttr(image) != 14) {
		printf(ANSI_COLOR_RED "numAttr_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(image);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "numAttr_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(image);
	return (1);
}

int assign_xml_svg_X_test1() {
	xmlNode* root = NULL;
	xmlDoc* doc = NULL;
	SVGimage* i = createSVGimage("../test/rec_with_units.svg");

	doc = xmlNewDoc((xmlChar*) "1.0");
	root = xmlNewNode(NULL, (xmlChar *) "svg");
	xmlDocSetRootElement(doc, root);

	assign_xml_svg_basic(i, root);
	assign_xml_svg_rectangles(i, root);
	assign_xml_svg_circles(i, root);
	assign_xml_svg_paths(i, root);

	if (strcmp((char*)root->ns->href, i->namespace)) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->title[0] != '\0') && strcmp(i->title, (char*)root->children->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->description[0] != '\0') && strcmp(i->description, (char*)root->children->next->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test1" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	/* This relieves the memory leak but also creates an svg image in /bin */
	xmlSaveFormatFileEnc("rec_with_units_copy.svg", doc, "UTF-8", 1);

	printf(ANSI_COLOR_GREEN "assign_xml_svg_X_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i);
	xmlFreeDoc(doc);

	xmlCleanupParser();
	xmlMemoryDump();
	return (1);
}

int assign_xml_svg_X_test2() {
	xmlNode* root = NULL;
	xmlDoc* doc = NULL;
	SVGimage* i = createSVGimage("../test/Emoji_poo.svg");

	doc = xmlNewDoc((xmlChar*) "1.0");
	root = xmlNewNode(NULL, (xmlChar *) "svg");
	xmlDocSetRootElement(doc, root);

	assign_xml_svg_basic(i, root);
	assign_xml_svg_rectangles(i, root);
	assign_xml_svg_circles(i, root);
	assign_xml_svg_paths(i, root);
	assign_xml_svg_groups(i, root);


	if (strcmp((char*)root->ns->href, i->namespace)) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->title[0] != '\0') && strcmp(i->title, (char*)root->children->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->description[0] != '\0') && strcmp(i->description, (char*)root->children->next->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test2" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	xmlSaveFormatFileEnc("Emoji_poo_copy.svg", doc, "UTF-8", 1);

	printf(ANSI_COLOR_GREEN "assign_xml_svg_X_test2" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i);
	xmlFreeDoc(doc);

	xmlCleanupParser();
	xmlMemoryDump();
	return (1);
}

int assign_xml_svg_X_test3() {
	xmlNode* root = NULL;
	xmlDoc* doc = NULL;
	SVGimage* i = createSVGimage("../test/Brutal3.svg");

	doc = xmlNewDoc((xmlChar*) "1.0");
	root = xmlNewNode(NULL, (xmlChar *) "svg");
	xmlDocSetRootElement(doc, root);

	assign_xml_svg_basic(i, root);
	assign_xml_svg_rectangles(i, root);
	assign_xml_svg_circles(i, root);
	assign_xml_svg_paths(i, root);
	assign_xml_svg_groups(i, root);

	if (strcmp((char*)root->ns->href, i->namespace)) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->title[0] != '\0') && strcmp(i->title, (char*)root->children->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	if ( (i->description[0] != '\0') && strcmp(i->description, (char*)root->children->next->children->content) ) {
		printf(ANSI_COLOR_RED "assign_xml_svg_X_test3" ANSI_COLOR_RESET "\n");
		deleteSVGimage(i);
		return (0);
	}

	xmlSaveFormatFileEnc("Brutal3_copy.svg", doc, "UTF-8", 1);

	printf(ANSI_COLOR_GREEN "assign_xml_svg_X_test3" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i);
	xmlFreeDoc(doc);

	xmlCleanupParser();
	xmlMemoryDump();
	return (1);
}

int validateSVGimage_test1() {
	SVGimage* i = createSVGimage("../test/Emoji_poo.svg");

	if (!validateSVGimage(i, "../test/schemaFiles/svg.xsd")) {
		printf(ANSI_COLOR_RED "validateSVGimage_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "validateSVGimage_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i);
	return (1);
}

int validateSVGimage_test2() {
	SVGimage* i = createSVGimage("../test/Emoji_grinning_BAD.svg");

	if (validateSVGimage(i, "../test/schemaFiles/svg.xsd")) {
		printf(ANSI_COLOR_RED "validateSVGimage_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "validateSVGimage_test2" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i);
	return (1);
}

int createValidSVGimage_test1() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/Emoji_poo.svg", "../test/schemaFiles/svg.xsd");
	if (img) {
		printf(ANSI_COLOR_RED "createValidSVGimage_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "createValidSVGimage_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	return (1);
}

int createValidSVGimage_test2() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/rect.svg", "../test/schemaFiles/svg.xsd");
	if (!img) {
		printf(ANSI_COLOR_RED "createValidSVGimage_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "createValidSVGimage_test2" ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	return (1);
}

int writeSVGimage_test1() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/quad01.svg", "../test/schemaFiles/svg.xsd");
	if (!img) {
		printf(ANSI_COLOR_RED "writeSVGimage_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!writeSVGimage(img, "quad01_copy.svg")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "writeSVGimage_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int writeSVGimage_test2() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/Emoji_grinning.svg", "../test/schemaFiles/svg.xsd");
	if (!img) {
		printf(ANSI_COLOR_RED "writeSVGimage_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!validateSVGimage(img, "../test/schemaFiles/svg.xsd")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!writeSVGimage(img, "Emoji_grinning_copy.svg")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test2" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "writeSVGimage_test2" ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	return (1);
}

int writeSVGimage_test3() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/vest.svg", "../test/schemaFiles/svg.xsd");
	if (!img) {
		printf(ANSI_COLOR_RED "writeSVGimage_test3" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!writeSVGimage(img, "vest_copy.svg")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test3" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "writeSVGimage_test3" ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	return (1);
}

int writeSVGimage_test4() {
	SVGimage* img = NULL;
	img = createValidSVGimage("../test/beer.svg", "../test/schemaFiles/svg.xsd");
	if (!img) {
		printf(ANSI_COLOR_RED "writeSVGimage_test4" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!writeSVGimage(img, "beer_copy.svg")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test4" ANSI_COLOR_RESET "\n");
		return (0);
	}

	SVGimage* img2 = NULL;
	img2 = createValidSVGimage("beer_copy.svg", "../test/schemaFiles/svg.xsd");
	if (!img2) {
		printf(ANSI_COLOR_RED "writeSVGimage_test4" ANSI_COLOR_RESET "\n");
		return (0);
	}

	if (!writeSVGimage(img2, "beer_copyOfCopy.svg")) {
		printf(ANSI_COLOR_RED "writeSVGimage_test4" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "writeSVGimage_test4" ANSI_COLOR_RESET "\n");
	deleteSVGimage(img);
	deleteSVGimage(img2);
	return (1);
}

int setAttribute_test1() {
	SVGimage* img = createValidSVGimage("../test/rect.svg", "../test/schemaFiles/svg.xsd");

	setAttribute(img, SVG_IMAGE, 0, Attribute_Constructor("height", "12cm"));
	setAttribute(img, SVG_IMAGE, 0, Attribute_Constructor("width", "6cm"));
	setAttribute(img, SVG_IMAGE, 100, Attribute_Constructor("authorName", "James"));
	setAttribute(img, SVG_IMAGE, 100, Attribute_Constructor("xmlns", "www.google.ca"));

	if (!writeSVGimage(img, "rect_setAttribute_test1.svg")) {
		printf(ANSI_COLOR_RED "setAttribute_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "setAttribute_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int setAttribute_test2() {
	SVGimage* img = createValidSVGimage("../test/accessible.svg", "../test/schemaFiles/svg.xsd");

	setAttribute(img, CIRC, 0, Attribute_Constructor("CircleStatus", "good"));
	setAttribute(img, CIRC, 0, Attribute_Constructor("yada", "yeed"));
	setAttribute(img, CIRC, 0, Attribute_Constructor("cx", "123"));
	setAttribute(img, CIRC, 0, Attribute_Constructor("fill", "#6969"));
	//printf("could not find so inserting [%s] to back\n", newAttribute->name);

	writeSVGimage(img, "accesssible_setAttribute_test2.svg");

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "setAttribute_test2" ANSI_COLOR_RESET "\n");
	return (1);
}

int setAttribute_test3() {
	SVGimage* img = createValidSVGimage("../test/rect.svg", "../test/schemaFiles/svg.xsd");

	setAttribute(img, RECT, 0, Attribute_Constructor("RectangleStatus", "bad"));
	setAttribute(img, RECT, 0, Attribute_Constructor("joe", "boom"));
	setAttribute(img, RECT, 0, Attribute_Constructor("x", "123cm"));
	setAttribute(img, RECT, 0, Attribute_Constructor("y", "321cm"));
	setAttribute(img, RECT, 0, Attribute_Constructor("height", "33"));
	setAttribute(img, RECT, 0, Attribute_Constructor("width", "22inch"));
	setAttribute(img, RECT, 0, Attribute_Constructor("fill", "#1337"));

	writeSVGimage(img, "rect_setAttribute_test3.svg");

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "setAttribute_test3" ANSI_COLOR_RESET "\n");
	return (1);
}

int setAttribute_test4() {
	SVGimage* img = createValidSVGimage("../test/accessible.svg", "../test/schemaFiles/svg.xsd");

	setAttribute(img, PATH, 2, Attribute_Constructor("PathStatus", "healthy"));
	setAttribute(img, PATH, 1, Attribute_Constructor("nick", "f"));
	setAttribute(img, PATH, 1, Attribute_Constructor("d", "fconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua"));
	setAttribute(img, PATH, 1, Attribute_Constructor("height", "33"));
	setAttribute(img, PATH, 1, Attribute_Constructor("fill", "#AAAA"));

	writeSVGimage(img, "accessible_setAttribute_test4.svg");

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "setAttribute_test4" ANSI_COLOR_RESET "\n");
	return (1);
}

int setAttribute_test5() {
	SVGimage* img = createValidSVGimage("../test/quad01.svg", "../test/schemaFiles/svg.xsd");

	setAttribute(img, GROUP, 0, Attribute_Constructor("groupStatus", "happy"));
	setAttribute(img, GROUP, 1, Attribute_Constructor("groupStatus", "melon"));
	setAttribute(img, GROUP, 1, Attribute_Constructor("fill", "white"));
	setAttribute(img, GROUP, 2, Attribute_Constructor("fill", "#99999"));

	writeSVGimage(img, "quad01_setAttribute_test5.svg");

	deleteSVGimage(img);
	printf(ANSI_COLOR_GREEN "setAttribute_test5" ANSI_COLOR_RESET "\n");
	return (1);
}

int addComponent_test1() {
	SVGimage* i =  createValidSVGimage("../test/quad01.svg", "../test/schemaFiles/svg.xsd");

	Rectangle* component1 = Rectangle_Contructor();
	component1->height = 12.00f;
	component1->width = 3.14f;
	component1->x = 9.2f;
	component1->y = 10.2f;
	addComponent(i, RECT, (void *)component1);

	Path* component2 = Path_Constructor();
	char* data = "N1 32 75 E5 dsds";
	component2->data = malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(component2->data, data);
	addComponent(i, PATH, component2);
	writeSVGimage(i, "quad01_addComponent_test1.svg");

	deleteSVGimage(i);
	printf(ANSI_COLOR_GREEN "addComponent_test1" ANSI_COLOR_RESET "\n");
	return (1);
}

int circleToJSON_test1() {
	Circle* c = Circle_Constructor();
	c->cx = 1.23f;
	c->cy = 64.3f;
	c->r = 89.232f;
	strcpy(c->units, "feet");
	insertBack(c->otherAttributes, Attribute_Constructor("stroke", "full"));

	char* cs = circleToJSON(c);
	char* cse = "{\"cx\":1.23,\"cy\":64.30,\"r\":89.23,\"numAttr\":1,\"units\":\"feet\"}";
	
	printf("Expected %s\n", cse);
	printf("Received %s\n", cs);
	if (strcmp(cse, cs)) {
		printf("Not the same!\n");
		printf(ANSI_COLOR_RED "circleToJSON_test1" ANSI_COLOR_RESET "\n");
		deleteCircle(c);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "circleToJSON_test1" ANSI_COLOR_RESET "\n");
	free(cs);
	deleteCircle(c);
	return (1);
}

int rectToJSON_test1() {
	Rectangle* r = Rectangle_Contructor();
	r->x = 1;
	r->y = 2;
	r->width = 19;
	r->height = 15;
	strcpy(r->units, "cm");
	insertBack(r->otherAttributes, Attribute_Constructor("fill", "none"));
	insertBack(r->otherAttributes, Attribute_Constructor("stroke", "blue"));
	insertBack(r->otherAttributes, Attribute_Constructor("stroke-width", "1"));

	char* rs = rectToJSON(r);
	char* rse = "{\"x\":1.00,\"y\":2.00,\"w\":19.00,\"h\":15.00,\"numAttr\":3,\"units\":\"cm\"}";
	
	printf("Expected %s\n", rse);
	printf("Received %s\n", rs);
	if (strcmp(rse, rs)) {
		printf("Not the same!\n");
		printf(ANSI_COLOR_RED "rectleToJSON_test1" ANSI_COLOR_RESET "\n");
		deleteRectangle(r);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "rectToJSON_test1" ANSI_COLOR_RESET "\n");
	deleteRectangle(r);
	free(rs);
	return (1);
}

int pathToJSON_test1() {
	Path* p = Path_Constructor();
	char* d = "m47 36c-15 0-15 0-29.9 0-2.1 0-2.1 4-.1 4 10.4 0 19.6 0 30 0 2 0 2-4 0-4";
	p->data = malloc(sizeof(char) * (strlen(d) + 1));
	strcpy(p->data, d);
	insertBack(p->otherAttributes, Attribute_Constructor("fill", "#fff"));

	char* s = pathToJSON(p);
	char* se = "{\"d\":\"m47 36c-15 0-15 0-29.9 0-2.1 0-2.1 4-.1 4 10.4 0 19.6 0 30 0 2 0\",\"numAttr\":1}";
	
	printf("Expected %s\n", se);
	printf("Received %s\n", s);
	if (strcmp(se, s)) {
		printf("Not the same!\n");
		printf(ANSI_COLOR_RED "pathleToJSON_test1" ANSI_COLOR_RESET "\n");
		deletePath(p);
		return (0);
	}

	printf(ANSI_COLOR_GREEN "pathToJSON_test1" ANSI_COLOR_RESET "\n");
	free(s);
	deletePath(p);
	return (1);
}

int circListToJSON_test1() {
	SVGimage* i = createValidSVGimage("../test/quad01.svg", "../test/schemaFiles/svg.xsd");

	List* cl = getCircles(i);
	char* s = circListToJSON(cl);
	printf("%s\n", s);

	List* t = initializeList(circleToString, deleteCircle, compareCircles);
	char* s2 = circListToJSON(t);
	printf("%s\n", s2);

	printf(ANSI_COLOR_GREEN "circListToJSON_test1" ANSI_COLOR_RESET "\n");
	free(s);
	free(s2);
	freeList(cl);
	freeList(t);
	deleteSVGimage(i);
	return (1);
}

int JSONtoSVG_test1() {
	char* s1 = "{\"title\":\"titleVal\",\"descr\":\"descrVal\"}";
	SVGimage* i1 = JSONtoSVG(s1);
	if (
		strcmp(i1->namespace, "http://www.w3.org/2000/svg") ||
		strcmp(i1->title, "titleVal") ||
		strcmp(i1->description, "descrVal")
	) {
		printf(ANSI_COLOR_RED "JSONtoSVG_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* s2 = "{\"title\":\"\",\"descr\":\"\"}";
	SVGimage* i2 = JSONtoSVG(s2);
	if (
		strcmp(i2->namespace, "http://www.w3.org/2000/svg") ||
		strcmp(i2->title, "") ||
		strcmp(i2->description, "")
	) {
		printf("%s\n%s\n%s\n", i2->namespace, i2->title, i2->description);
		printf(ANSI_COLOR_RED "JSONtoSVG_test1" ANSI_COLOR_RESET "\n");

		return (0);
	}

	char* s3 = "{\"title\":\"titleVal\",\"descr\":\"\"}";
	SVGimage* i3 = JSONtoSVG(s3);
	if (
		strcmp(i3->namespace, "http://www.w3.org/2000/svg") ||
		strcmp(i3->title, "titleVal") ||
		strcmp(i3->description, "")
	) {
		printf("%s\n%s\n%s\n", i3->namespace, i3->title, i3->description);
		printf(ANSI_COLOR_RED "JSONtoSVG_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* s4 = "{\"title\":\"\",\"descr\":\"junk\"}";
	SVGimage* i4 = JSONtoSVG(s4);
	if (
		strcmp(i4->namespace, "http://www.w3.org/2000/svg") ||
		strcmp(i4->title, "") ||
		strcmp(i4->description, "junk")
	) {
		printf("%s\n%s\n%s\n", i4->namespace, i4->title, i4->description);
		printf(ANSI_COLOR_RED "JSONtoSVG_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "JSONtoSVG_test1" ANSI_COLOR_RESET "\n");
	deleteSVGimage(i1);
	deleteSVGimage(i2);
	deleteSVGimage(i3);
	deleteSVGimage(i4);
	return (1);
}

int JSONtoRect_test1() {
	char* s1 = "{\"x\":1,\"y\":2,\"w\":19,\"h\":15,\"units\":\"cm\"}";
	Rectangle* r = JSONtoRect(s1);

	if (
		r->x != 1.000000f ||
		r->y != 2.000000f ||
		r->width != 19.000000f ||
		r->height != 15.000000f ||
		strcmp(r->units, "cm")
	) {
		printf("%f\n%f\n%f\n%f\n[%s]\n", r->x, r->y, r->width, r->height, r->units);
		printf(ANSI_COLOR_RED "JSONtoRect_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	char* s2 = "{\"x\":0.32,\"y\":222.0,\"w\":12.65,\"h\":153,\"units\":\"Inches\"}";
	Rectangle* r2 = JSONtoRect(s2);

	if (r2->x != 0.32f ||
		r2->y != 222.0f ||
		r2->width != 12.65f ||
		r2->height != 153.000f ||
		strcmp(r2->units, "Inches")
	) {
		printf("x:%f\ny:%f\nwidth:%f\nheight:%f\nunits[%s]\n", r2->x, r2->y, r2->width, r2->height, r2->units);
		printf(ANSI_COLOR_RED "JSONtoRect_test1" ANSI_COLOR_RESET "\n");
		return (0);
	}

	printf(ANSI_COLOR_GREEN "JSONtoRect_test1" ANSI_COLOR_RESET "\n");
	deleteRectangle(r);
	deleteRectangle(r2);
	return (1);
}

int JSONtoCircle_test1() {
	char* s1 = "{\"cx\":32,\"cy\":32,\"r\":12.65,\"units\":\"\"}";
	Circle * c = JSONtoCircle(s1);

	if (c->cx != 32.0f ||
		c->cy != 32.0f ||
		c->r != 12.65f ||
		c->units[0] != '\0'
	) {
		printf("cx:%f\ncy:%f\nr:%f\nunits:[%s]\n", c->cx, c->cy, c->r, c->units);
		printf(ANSI_COLOR_RED "JSONtoCircle_test1" ANSI_COLOR_RESET "\n");
	}
	
	char* s2 = "{\"cx\":-2.21,\"cy\":6433.0,\"r\":1.1,\"units\":\"feet\"}";
	Circle * c2 = JSONtoCircle(s2);

	if (c2->cx != -2.21f ||
		c2->cy != 6433.0f ||
		c2->r != 1.1f ||
		strcmp(c2->units, "feet")
	) {
		printf("cx:%f\ncy:%f\nr:%f\nunits:[%s]\n", c2->cx, c2->cy, c2->r, c2->units);
		printf(ANSI_COLOR_RED "JSONtoCircle_test1" ANSI_COLOR_RESET "\n");
	}
	
	printf(ANSI_COLOR_GREEN "JSONtoCircle_test1" ANSI_COLOR_RESET "\n");
	deleteCircle(c);
	deleteCircle(c2);
	return(1);
} 

#define TOTAL_TESTS 53

int main(void) {

	int passed = 0;

	passed += createSVGimage_test1();
	passed += createSVGimage_test2();
	passed += createSVGimage_test3();
	passed += createSVGimage_test4();

	passed += SVGimage_Constructor_test1();

	passed += deleteSVGimage_test1();

	passed += SVGimageToString_test1();
	passed += SVGimageToString_test2();
	passed += SVGimageToString_test3();
	passed += SVGimageToString_test4();

	passed += Attribute_Constructor_test1();
	passed += Attribute_Constructor_test2();

	passed += Rectangle_Contructor_test1();
	passed += rectangleToString_test1();
	passed += compareRectangles_test1();

	passed += Circle_Constructor_test1();
	passed += circleToString_test1();

	passed += pathToString_test1();

	passed += Group_Constructor_test1();

	passed += getRects_test1();
	passed += getCircles_test1();
	passed += getPaths_test1();
	passed += getGroups_test1();

	passed += numRectsWithArea_test1();
	passed += numCirclesWithArea_test1();
	passed += numPathsWithdata_test1();
	passed += numGroupsWithLen_test1();
	passed += numAttr_test1();
	passed += numAttr_test2();

	passed += assign_xml_svg_X_test1();
	passed += assign_xml_svg_X_test2();
	passed += assign_xml_svg_X_test3();

	passed += validateSVGimage_test1();
	passed += validateSVGimage_test2();
	passed += createValidSVGimage_test1();
	passed += createValidSVGimage_test2();

	passed += writeSVGimage_test1();
	passed += writeSVGimage_test2();
	passed += writeSVGimage_test3();
	passed += writeSVGimage_test4();

	passed += setAttribute_test1();
	passed += setAttribute_test2();
	passed += setAttribute_test3();
	passed += setAttribute_test4();
	passed += setAttribute_test5();

	passed += addComponent_test1();

	passed += circleToJSON_test1();
	passed += rectToJSON_test1();
	passed += pathToJSON_test1();

	passed += circListToJSON_test1();

	passed += JSONtoSVG_test1();
	passed += JSONtoRect_test1();
	passed += JSONtoCircle_test1();

	printf(ANSI_COLOR_RED   "*** FAILED %d TESTS ***"   ANSI_COLOR_RESET "\n", TOTAL_TESTS - passed);
	printf(ANSI_COLOR_GREEN "*** PASSED %d TESTS ***"   ANSI_COLOR_RESET "\n", passed);

	return (1);
}