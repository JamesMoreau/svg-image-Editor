/**
 * @file SVGParser.h
 * @author James Moreau
 * @ID 1065510
 * @E jmorea03@uoguelph.ca
 */

#include "SVGParser.h"

#ifndef __HELPER__
#define __HELPER__

/* Misc */
void dummy_delete(void* d);
char *strdup(const char *c);

/* Constructors */
Attribute* Attribute_Constructor(char* name, char* value);
SVGimage *SVGimage_Constructor(void);
Rectangle* Rectangle_Contructor(void);
Circle* Circle_Constructor(void);
Path* Path_Constructor(void);
Group* Group_Constructor(void);

/* Parsing */
void parse_SVGimage(SVGimage* img, xmlNode* a_node);
void find_namespace(SVGimage* img, xmlNode* r);
void parse_SVGImage_attributes(xmlNode* node, SVGimage* img);
Rectangle* parse_rectangle(xmlNode* node);
Circle* parse_circle(xmlNode* node);
Path* parse_path(xmlNode* node);
Group* parse_group(xmlNode* a_node);
xmlDoc* create_SVG_Tree(SVGimage* image);

/* recursive parsing */
void recursive_rectangles(List* l, Group* g);
void recursive_circles(List* l, Group* g);
void recursive_paths(List* l, Group* g);
void recursive_groups(List* l, Group* g);

/* making xml / validation */
bool assign_xml_svg_basic(SVGimage* img, xmlNode* r);
bool assign_xml_svg_rectangles(SVGimage* img, xmlNode* r);
bool assign_xml_svg_circles(SVGimage* img, xmlNode* r);
bool assign_xml_svg_paths(SVGimage* img, xmlNode* r);
bool assign_xml_svg_groups(SVGimage* img, xmlNode* r);
bool assign_xml_recursive_groups(Group* g, xmlNode* groupNode);
bool validate_xml(xmlDoc* doc, char* schema);

#endif