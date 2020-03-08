/**
 * @file SVGParser.c
 * @author James Moreau
 * @ID 1065510
 * @E jmorea03@uoguelph.ca
 */

#include "SVGParser.h"
#include <libxml/xmlschemastypes.h>
#include "Helpers.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

SVGimage* createSVGimage(char* fileName) {
	if (!fileName || (strcmp(fileName, "") == 0)) return (NULL);

	SVGimage* myImage = NULL;
	xmlDoc* f = NULL;
	xmlNode* root = NULL;

	f = xmlReadFile(fileName, NULL, 0);
	if (!f) return (NULL);

	root = xmlDocGetRootElement(f);
	if (!root) return (NULL);

	myImage = SVGimage_Constructor(); //SVGimage beyond this point must be freed

	find_namespace(myImage, root);
	if (myImage->namespace[0] == '\0') {
		deleteSVGimage(myImage);
		return (NULL);
	}

	parse_SVGimage(myImage, root);

	xmlFreeDoc(f);
	xmlCleanupParser();

	return (myImage);
}

char* SVGimageToString(SVGimage* img) {
	if (!img) return (NULL);

	char s[20000]; s[0] = '\0';

	strcat(s,"SVGimage:\n");

	strcat(s, " namespace: "); strcat(s, img->namespace); strcat(s, "\n");

	strcat(s, " title: "); strcat(s, img->title); strcat(s, "\n");

	strcat(s, " description: "); strcat(s, img->description); strcat(s, "\n");

	char* Rectangles_Str = toString(img->rectangles);
	strcat(s, " rectangles:"); strcat(s, Rectangles_Str); strcat(s, "\n");
	free(Rectangles_Str);

	char* circles_String = toString(img->circles);
	strcat(s, " circles:"); strcat(s, circles_String); strcat(s, "\n");
	free(circles_String);

	char* paths_String = toString(img->paths);
	strcat(s, " paths:"); strcat(s, paths_String); strcat(s, "\n");
	free(paths_String);

	char* groups_String = toString(img->groups);
	strcat(s, "groups: "); strcat(s, groups_String); strcat(s, "\n");
	free(groups_String);

	char* attributes_String = toString(img->otherAttributes);
	strcat(s, " other attributes: "); strcat(s, attributes_String); strcat(s, "\n");
	free(attributes_String);

	return strdup(s);
}

void deleteSVGimage(SVGimage* img) {
	if (!img) return;

	if (img->circles) freeList(img->circles);

	if (img->rectangles) freeList(img->rectangles);

	if (img->paths) freeList(img->paths);

	if (img->groups) freeList(img->groups);

	if (img->otherAttributes) freeList(img->otherAttributes);

	free(img);
}

List* getRects(SVGimage* img) {
	if (!img) return (NULL);

	List* l = initializeList(rectangleToString,
							 dummy_delete,
							 compareRectangles);

	ListIterator I = createIterator(img->rectangles);
	Rectangle* ptr;

	while((ptr = (Rectangle*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(img->groups);
	Group* gPtr;

	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_rectangles(l, gPtr);
	}

	return (l);
}

List* getCircles(SVGimage* img) {
	if (!img) return (NULL);

	List* l = initializeList(circleToString, dummy_delete, compareCircles);
	ListIterator I = createIterator(img->circles);
	Circle* ptr;

	while((ptr = (Circle*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(img->groups);
	Group* gPtr;

	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_circles(l, gPtr);
	}

	return (l);
}

List* getGroups(SVGimage* img) {
	if (!img) return (NULL);

	List* l = initializeList(groupToString, dummy_delete, compareGroups);

	ListIterator I = createIterator(img->groups);
	Group* ptr;
	while((ptr = (Group*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
		recursive_groups(l, ptr);
	}

	return (l);
}

List* getPaths(SVGimage* img) {
	if (!img) return (NULL);

	List* l = initializeList(pathToString, dummy_delete, comparePaths);
	ListIterator I = createIterator(img->paths);
	Path* ptr;

	while((ptr = (Path*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(img->groups);
	Group* gPtr;

	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_paths(l, gPtr);
	}

	return (l);
}

int numRectsWithArea(SVGimage* img, float area) {
	if (!img || (area<0)) return (0);

	List* l = getRects(img);
	int rCounter = 0;

	ListIterator I = createIterator(l);
	Rectangle* ptr;
	while((ptr = (Rectangle*)nextElement(&I)) != NULL) {
		if (ceil((ptr->height) * (ptr->width)) == ceil(area)) {
			rCounter++;
		}
	}

	freeList(l);
	return (rCounter);
}

int numCirclesWithArea(SVGimage* img, float area) {
	if (!img || (area<0)) return (0);

	List* l = getCircles(img);
	int cCounter = 0;

	ListIterator I = createIterator(l);
	Circle* ptr;
	while((ptr = (Circle*)nextElement(&I)) != NULL) {
		if (ceil((ptr->r) * (ptr->r) * (3.1415926535)) == ceil(area)) {
			cCounter++;
		}
	}

	freeList(l);
	return (cCounter);
}

int numPathsWithdata(SVGimage* img, char* data) {
	if (!img || !data) return (0);

	List* l = getPaths(img);
	int pCounter = 0;
	ListIterator I = createIterator(l);
	Path* ptr;

	while((ptr = (Path*)nextElement(&I)) != NULL) {
		if (strcmp(ptr->data, data) == 0) {
			pCounter++;
		}
	}

	freeList(l);
	return (pCounter);
}

int numGroupsWithLen(SVGimage* img, int len) {
	if (!img || (len<0)) return (0);

	List* l = getGroups(img);
	int gCounter = 0;
	int currentLength = 0;
	ListIterator I = createIterator(l);
	Group* ptr;

	while((ptr = (Group*)nextElement(&I)) != NULL) {
		currentLength = getLength(ptr->rectangles) +
						getLength(ptr->circles) +
						getLength(ptr->paths) +
						getLength(ptr->groups);

		if (currentLength == len) {
			gCounter++;
		}
	}

	freeList(l);
	return (gCounter);
}

int numAttr(SVGimage* img) {
	if (!img) return (0);

	int counter = 0;

	counter += getLength(img->otherAttributes);

	List* r = getRects(img);
	ListIterator rI = createIterator(r);
	Rectangle* rPtr;
	while((rPtr = (Rectangle*)nextElement(&rI)) != NULL) {
		counter += getLength(rPtr->otherAttributes);
	}
	freeList(r);

	List* c = getCircles(img);
	ListIterator cI = createIterator(c);
	Circle* cPtr;
	while((cPtr = (Circle*)nextElement(&cI)) != NULL) {
		counter += getLength(cPtr->otherAttributes);
	}
	freeList(c);

	List* p = getPaths(img);
	ListIterator pI = createIterator(p);
	Path* pPtr;
	while((pPtr = (Path*)nextElement(&pI)) != NULL) {
		counter += getLength(pPtr->otherAttributes);
	}
	freeList(p);

	List* g = getGroups(img);
	ListIterator gI = createIterator(g);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&gI)) != NULL) {
		counter += getLength(gPtr->otherAttributes);
	}
	freeList(g);

	return (counter);
}

bool validateSVGimage(SVGimage* image, char* schemaFile) {
	if (!image || !schemaFile || (strcmp(schemaFile, "") == 0)) return false;

	xmlLineNumbersDefault(1);
	xmlDoc* doc = create_SVG_Tree(image);
	if (!doc) return (false);
	if (!validate_xml(doc, schemaFile)) return (false);

	return (true);
}

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
	if (
		!fileName ||
		!schemaFile ||
		(strcmp(fileName, "") == 0) ||
		(strcmp(schemaFile, "") == 0)
	) return (NULL);

	if(!strstr(fileName, ".svg")) return (NULL);

	xmlDoc* doc = xmlReadFile(fileName, NULL, 0);
	if (!validate_xml(doc, schemaFile)) return (NULL);

	SVGimage* img = NULL;
	img = createSVGimage(fileName);
	if (!img) return (NULL);

	if(!validateSVGimage(img, schemaFile)) return (NULL);

	return (img);
}

bool writeSVGimage(SVGimage* image, char* fileName) {
	if (!image || !fileName) return (false);

	if (!strstr(fileName, ".svg")) return (false);

	xmlDoc* doc = create_SVG_Tree(image);
	if (!doc) return (false);

	/* Writing file */
	xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

	/* Cleaning up */
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();

	return (true);
}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute) {
	if (!image || !newAttribute || !newAttribute->name || !newAttribute->value) return;

	bool added = false;

	if (elemType == SVG_IMAGE) {
		if (strcmp(newAttribute->name, "xmlns") == 0) {
			strncpy(image->namespace, newAttribute->value, 255);

		} else {
			ListIterator aI = createIterator(image->otherAttributes);
			Attribute* aPtr;
			while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
				if (strcmp(aPtr->name, newAttribute->name) == 0) {
					free(aPtr->value);
					aPtr->value = malloc(sizeof(char) *
										(strlen(newAttribute->value) + 1));
					strcpy(aPtr->value, newAttribute->value);
					added = true;
				}
			}

			if (!added) {
				insertBack(image->otherAttributes,
						   Attribute_Constructor(newAttribute->name,
						   						 newAttribute->value));
			}
		}
		deleteAttribute(newAttribute);
	} else if (elemType == CIRC) {
			ListIterator cI = createIterator(image->circles);
			Circle* cPtr;
			while((cPtr = (Circle*)nextElement(&cI)) != NULL) {
				if (elemIndex == 0) {
					if (strcmp(newAttribute->name, "cx") == 0) {
						cPtr->cx = strtof(newAttribute->value, NULL);

					} else if (strcmp(newAttribute->name, "cy") == 0) {
						cPtr->cy = strtof(newAttribute->value, NULL);

					} else if (strcmp(newAttribute->name, "r") == 0) {
						cPtr->r = strtof(newAttribute->value, NULL);

					} else {
						ListIterator aI = createIterator(cPtr->otherAttributes);
						Attribute* aPtr;
						while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
							if (strcmp(aPtr->name, newAttribute->name) == 0) {
								free(aPtr->value);
								aPtr->value = malloc(sizeof(char) *
													(strlen(newAttribute->value) + 1));
								strcpy(aPtr->value, newAttribute->value);
								added = true;
							}
						}

						if (!added) {
							insertBack(cPtr->otherAttributes,
									   Attribute_Constructor(newAttribute->name,
									   						 newAttribute->value));
						}
					}
					deleteAttribute(newAttribute);
				}
				elemIndex--;
			}
	} else if (elemType == RECT) {
		ListIterator rI = createIterator(image->rectangles);
		Rectangle* rPtr;
		while((rPtr = (Rectangle*)nextElement(&rI)) != NULL) {
			if (elemIndex == 0)	{
				if (strcmp(newAttribute->name, "x") == 0) {
					rPtr->x = strtof(newAttribute->value, NULL);

				} else if (strcmp(newAttribute->name, "y") == 0) {
					rPtr->y = strtof(newAttribute->value, NULL);

				} else if (strcmp(newAttribute->name, "width") == 0) {
					rPtr->width = strtof(newAttribute->value, NULL);

				} else if (strcmp(newAttribute->name, "height") == 0) {
					rPtr->height = strtof(newAttribute->value, NULL);

				} else {
					ListIterator aI = createIterator(rPtr->otherAttributes);
					Attribute* aPtr;
					while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
						if (strcmp(aPtr->name, newAttribute->name) == 0) {
							free(aPtr->value);
							aPtr->value = malloc(sizeof(char) *
												(strlen(newAttribute->value) + 1));
							strcpy(aPtr->value, newAttribute->value);
							added = true;
						}
					}

					if (!added) {
						insertBack(rPtr->otherAttributes,
								   Attribute_Constructor(newAttribute->name,
								   						 newAttribute->value));
					}
				}
				deleteAttribute(newAttribute);
			}
			elemIndex--;
		}
	} else if (elemType == PATH) {
		ListIterator pI = createIterator(image->paths);
		Path* pPtr;
		while((pPtr = (Path*)nextElement(&pI)) != NULL) {
			if (elemIndex == 0)	{
				if (strcmp(newAttribute->name, "d") == 0) {
					free(pPtr->data);
					pPtr->data = malloc(sizeof(char) *
									   (strlen(newAttribute->value) + 1));
					strcpy(pPtr->data, newAttribute->value);

				} else {
					ListIterator aI = createIterator(pPtr->otherAttributes);
					Attribute* aPtr;
					while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
						if (strcmp(aPtr->name, newAttribute->name) == 0) {
							free(aPtr->value);
							aPtr->value = malloc(sizeof(char) *
												(strlen(newAttribute->value) + 1));
							strcpy(aPtr->value, newAttribute->value);
							added = true;
						}
					}

					if (!added) {
						insertBack(pPtr->otherAttributes,
								   Attribute_Constructor(newAttribute->name,
								   						 newAttribute->value));
					}
				}
				deleteAttribute(newAttribute);
			}
			elemIndex--;
		}
	} else if (elemType == GROUP) {
		ListIterator gI = createIterator(image->groups);
		Group* gPtr;
		while((gPtr = (Group*)nextElement(&gI)) != NULL) {
			if (elemIndex == 0)	{
				ListIterator aI = createIterator(gPtr->otherAttributes);
				Attribute* aPtr;
				while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
					if (strcmp(aPtr->name, newAttribute->name) == 0) {
						free(aPtr->value);
						aPtr->value = malloc(sizeof(char) *
											(strlen(newAttribute->value) + 1));
						strcpy(aPtr->value, newAttribute->value);
						added = true;
					}
				}

				if (!added) {
					insertBack(gPtr->otherAttributes,
							   Attribute_Constructor(newAttribute->name,
							   						 newAttribute->value));
				}
				deleteAttribute(newAttribute);
			}
			elemIndex--;
		}
	}
}

void addComponent(SVGimage* image, elementType type, void* newElement) {
	if (!image || !newElement) return;

	if (type == CIRC) {
		insertBack(image->circles, newElement);
	} else if (type == RECT) {
		insertBack(image->rectangles, newElement);
	} else if (type == PATH) {
		insertBack(image->paths, newElement);
	}
}

char* attrToJSON(const Attribute *a) {
	if (!a) return (strdup("{}"));

	char b[5000]; b[0] = '\0';

	strcat(b, "{\"name\":\""); strcat(b, a->name); strcat(b, "\",");

	strcat(b, "\"value\":\""); strcat(b, a->value); strcat(b, "\"}");

	return (strdup(b));
}

char* circleToJSON(const Circle *c) {
	if (!c) return (strdup("{}"));

	char b[5000]; b[0] = '\0';

	char cxStr[100];
	sprintf(cxStr, "%.2f", c->cx);
	strcat(b, "{\"cx\":"); strcat(b, cxStr); strcat(b, ",");

	char cyStr[100];
	sprintf(cyStr, "%.2f", c->cy);
	strcat(b, "\"cy\":"); strcat(b, cyStr); strcat(b, ",");

	char rStr[100];
	sprintf(rStr, "%.2f", c->r);
	strcat(b, "\"r\":"); strcat(b, rStr); strcat(b, ",");

	char lenStr[100];
	sprintf(lenStr, "%d", getLength(c->otherAttributes));
	strcat(b, "\"numAttr\":"); strcat(b, lenStr); strcat(b, ",");

	strcat(b, "\"units\":\""); strcat(b, c->units); strcat(b, "\"}");

	return (strdup(b));
}

char* rectToJSON(const Rectangle *r) {
	if (!r) return (strdup("{}"));

	char b[5000]; b[0] = '\0';

	char xStr[100];
	sprintf(xStr, "%.2f", r->x);
	strcat(b, "{\"x\":"); strcat(b, xStr); strcat(b, ",");

	char yStr[100];
	sprintf(yStr, "%.2f", r->y);
	strcat(b, "\"y\":"); strcat(b, yStr); strcat(b, ",");

	char widthStr[100];
	sprintf(widthStr, "%.2f", r->width);
	strcat(b, "\"w\":"); strcat(b, widthStr); strcat(b, ",");

	char heightStr[100];
	sprintf(heightStr, "%.2f", r->height);
	strcat(b, "\"h\":"); strcat(b, heightStr); strcat(b, ",");

	char lenStr[100];
	sprintf(lenStr, "%d", getLength(r->otherAttributes));
	strcat(b, "\"numAttr\":"); strcat(b, lenStr); strcat(b, ",");

	strcat(b, "\"units\":\""); strcat(b, r->units); strcat(b, "\"}");

	return (strdup(b));
}

char* pathToJSON(const Path *p) {
	if (!p) return (strdup("{}"));

	char b[5000]; b[0] = '\0';

	strcat(b, "{\"d\":\""); strncat(b, p->data, 64); strcat(b, "\",");

	char lenStr[100];
	sprintf(lenStr, "%d", getLength(p->otherAttributes));
	strcat(b, "\"numAttr\":"); strcat(b, lenStr); strcat(b, "}");

	return (strdup(b));
}

char* groupToJSON(const Group *g) {
	if (!g) return (strdup("{}"));

	char b[5000]; b[0] = '\0';

	char numChildrenStr[100];
	sprintf(numChildrenStr, "%d", getLength(g->rectangles) +
								  getLength(g->circles) +
								  getLength(g->paths) +
								  getLength(g->groups));

	strcat(b, "{\"children\":"); strcat(b, numChildrenStr); strcat(b, ",");

	char attrLenStr[100];
	sprintf(attrLenStr, "%d", getLength(g->otherAttributes));
	strcat(b, "\"numAttr\":"); strcat(b, attrLenStr); strcat(b, "}");

	return (strdup(b));
}

char* attrListToJSON(const List *list) {
	List* l = (List*) list;
	if (!l || (getLength(l) == 0)) return (strdup("[]"));
	char b[5000]; b[0] = '\0';

	strcat(b, "[");

	ListIterator aI = createIterator(l);
	Attribute* aPtr;
	while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
		char* t = attrToJSON(aPtr);
		strcat(b, t);
		strcat(b, ",");
		free(t);
	}

	b[strlen(b)-1] = '\0'; strcat(b, "]");

	return (strdup(b));
}

char* circListToJSON(const List *list) {
	List* l = (List*) list;
	if (!l || (getLength(l) == 0)) return (strdup("[]"));
	char b[5000]; b[0] = '\0';

	strcat(b, "[");

	ListIterator cI = createIterator(l);
	Circle* cPtr;
	while((cPtr = (Circle*)nextElement(&cI)) != NULL) {
		char* t = circleToJSON(cPtr);
		strcat(b, t);
		strcat(b, ",");
		free(t);
	}

	b[strlen(b)-1] = '\0'; strcat(b, "]");

	return (strdup(b));
}

char* rectListToJSON(const List *list) {
	List* l = (List*) list;
	if (!l || (getLength(l) == 0)) return (strdup("[]"));
	char b[5000]; b[0] = '\0';

	strcat(b, "[");

	ListIterator rI = createIterator(l);
	Rectangle* rPtr;
	while((rPtr = (Rectangle*)nextElement(&rI)) != NULL) {
		char* t = rectToJSON(rPtr);
		strcat(b, t);
		strcat(b, ",");
		free(t);
	}

	b[strlen(b)-1] = '\0'; strcat(b, "]");

	return (strdup(b));
}

char* pathListToJSON(const List *list) {
	List* l = (List*) list;
	if (!l || (getLength(l) == 0)) return (strdup("[]"));
	char b[5000]; b[0] = '\0';

	strcat(b, "[");

	ListIterator pI = createIterator(l);
	Path* pPtr;
	while((pPtr = (Path*)nextElement(&pI)) != NULL) {
		char* t = pathToJSON(pPtr);
		strcat(b, t);
		strcat(b, ",");
	}

	b[strlen(b)-1] = '\0'; strcat(b, "]");

	return (strdup(b));
}

char* groupListToJSON(const List *list) {
	List* l = (List*) list;
	if (!l || (getLength(l) == 0)) return (strdup("[]"));
	char b[5000]; b[0] = '\0';

	strcat(b, "[");

	ListIterator gI = createIterator(l);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&gI)) != NULL) {
		char* t = groupToJSON(gPtr);
		strcat(b, t);
		strcat(b, ",");
		free(t);
	}

	b[strlen(b)-1] = '\0'; strcat(b, "]");

	return (strdup(b));
}

char* SVGtoJSON(const SVGimage* image) {
	if (!image) return (strdup("{}"));

	SVGimage* i = (SVGimage*) image;

	char b[5000]; b[0] = '\0';

	char numRectStr[100];
	List* rL = getRects(i);
	sprintf(numRectStr, "%d", getLength(rL));
	strcat(b, "{\"numRect\":"); strcat(b, numRectStr); strcat(b, ",");
	freeList(rL);

	char numCircStr[100];
	List* cL = getCircles(i);
	sprintf(numCircStr, "%d", getLength(cL));
	strcat(b, "\"numCirc\":"); strcat(b, numCircStr); strcat(b, ",");
	freeList(cL);

	char numPathsStr[100];
	List* pL = getPaths(i);
	sprintf(numPathsStr, "%d", getLength(pL));
	strcat(b, "\"numPaths\":"); strcat(b, numPathsStr); strcat(b, ",");
	freeList(pL);

	char numGroupsStr[100];
	List* gL = getGroups(i);
	sprintf(numGroupsStr, "%d", getLength(gL));
	strcat(b, "\"numGroups\":"); strcat(b, numGroupsStr); strcat(b, "}");
	freeList(gL);

	return (strdup(b));
}

char* SVGtoJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = SVGtoJSON(img);
	deleteSVGimage(img);
	return (s);
}

char* rectListToJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = rectListToJSON(img->rectangles);
	deleteSVGimage(img);
	return (s);
}

char* circListToJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = circListToJSON(img->circles);
	deleteSVGimage(img);
	return (s);
}

char* pathListToJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = pathListToJSON(img->paths);
	deleteSVGimage(img);
	return (s);
}

char* groupListToJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = groupListToJSON(img->groups);
	deleteSVGimage(img);
	return (s);
}

char* attrListToJSON_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	char* s = attrListToJSON(img->otherAttributes);
	deleteSVGimage(img);
	return (s);
}

char* SVG_get_title_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	if (!img) return (NULL);
	char* s = strdup(img->title);
	deleteSVGimage(img);
	return(s);
}

char* SVG_get_description_Wrapper(char* filename, char* schemaFile) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	if (!img) return (NULL);
	char* s = strdup(img->description);
	deleteSVGimage(img);
	return(s);
}

bool validateSVGimage_Wrapper(char* filename, char* schemaFile) {
	printf("filename: %s, schemaFile: %s\n", filename, schemaFile);
	SVGimage* img = createValidSVGimage(filename, schemaFile); //this calls validate svg implicitly
	if (!img) return (false);
	deleteSVGimage(img);
	return (true);
}

void setAttribute_Wrapper(char* filename, char* schemaFile, int elemType, int elemIndex, char* name, char* value) {
	printf("EDITING: elemType: %d, elemIndex: %d, name: %s, value: %s", elemType, elemIndex, name, value);
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	Attribute* a = Attribute_Constructor(name, value);
	setAttribute(img, elemType, elemIndex, a);
	writeSVGimage(img, filename);
	deleteSVGimage(img);
}

void setTitle_Wrapper(char* filename, char* schemaFile, char* value) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	strncpy(img->title, value, 255);
	writeSVGimage(img, filename);
	deleteSVGimage(img);
}

void setDescription_Wrapper(char* filename, char* schemaFile, char* value) {
	SVGimage* img = createValidSVGimage(filename, schemaFile);
	strncpy(img->description, value, 255);
	writeSVGimage(img, filename);
	deleteSVGimage(img);
}

void create_empty_svg_image_wrapper(char* filename) {
	printf("fileName: %s\n", filename);
	char* jsonSVG = "{\"title\":\"newImage\",\"descr\":\"Empty Title\"}";
	SVGimage* img = JSONtoSVG(jsonSVG);
	writeSVGimage(img, filename);
	deleteSVGimage(img);
}

SVGimage* JSONtoSVG(const char* svgString) {
	if (!svgString) return (NULL);
	char* s = strdup(svgString);
	SVGimage* img = SVGimage_Constructor();

	strncpy(img->namespace, "http://www.w3.org/2000/svg", 255);

	char* cur = s;

	for (int j = 0; j < 3; j++) {
		for (; (*cur) != '"'; cur++);
		cur++;
	}

	int titleSize;
	char* titlePtr = cur; //cur is at titleVal
	for (titleSize = 0; (*cur) != '"'; cur++, titleSize++);
	if (titleSize != 0) {
		strncpy(img->title, titlePtr, titleSize);
		img->title[titleSize] = '\0';
	}

	for (int j = 0; j < 4; j++) {
		for (; (*cur) != '"'; cur++);
		cur++;
	}

	int descSize;
	char* descPtr = cur; //cur is at descval
	for (descSize = 0; (*cur) != '"'; cur++, descSize++);
	if (descSize != 0) {
		strncpy(img->description, descPtr, descSize);
		img->description[descSize] = '\0';
	}

	free(s);
	return (img);
}

Rectangle* JSONtoRect(const char* svgString) {
	if (!svgString) return (NULL);
	char* s = strdup(svgString);
	Rectangle* r = Rectangle_Contructor();

	sscanf(s, "{\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f,\"units\":\"cm\"}",
	 		&(r->x),
	 		&(r->y),
			&(r->width),
			&(r->height));

	char* pch;
	strtok(s, "\"");
	for (int j = 0; j < 11; j++) {
		pch = strtok(NULL, "\"");
	}

	if (pch[0] != '}') strncpy(r->units, pch, 49);

	free(s);
	return (r);
}

Circle* JSONtoCircle(const char* svgString) {
	if (!svgString) return (NULL);
	char* s = strdup(svgString);
	Circle* c = Circle_Constructor();

	sscanf(s, "{\"cx\":%f,\"cy\":%f,\"r\":%f,\"units\":\"unitStr\"}",
			&(c->cx),
			&(c->cy),
			&(c->r));

	char* pch;
	strtok(s, "\"");
	for (int j = 0; j < 9; j++) {
		pch = strtok(NULL, "\"");
	}

	if (pch[0] != '}') strncpy(c->units, pch, 49);

	free(s);
	return (c);
}

/* List helper functions */
void deleteAttribute(void* data) {
	if (!data) return;

	Attribute* a = (Attribute *) data;

	free(a->name);
	free(a->value);

	free(a);
}

char* attributeToString(void* p) {
	if (!p) return (NULL);

	Attribute* a = (Attribute *) p;
	char s[5000]; s[0] = '\0';

	strcat(s,"Attribute:\n");

	strcat(s, " name: "); strcat(s, a->name); strcat(s, "\n");

	strcat(s, " value: "); strcat(s, a->value);

	return strdup(s);
}

int compareAttributes(const void* f, const void* s) {
	return (1);
}

void deleteGroup(void* data) {
	Group* g = (Group*) data;

	freeList(g->rectangles);
	freeList(g->circles);
	freeList(g->paths);
	freeList(g->groups);
	freeList(g->otherAttributes);
	free(g);
}

char* groupToString(void* data) {
	if (!data) return (NULL);

	Group* g = (Group*) data;
	char s[6000]; s[0] = '\0';

	strcat(s,"\nGroup:\n");

	char* rectangleString = toString(g->rectangles);
	strcat(s, " rectangles:"); strcat(s, rectangleString); strcat(s, "\n");
	free(rectangleString);

	char* circlesString = toString(g->circles);
	strcat(s, " circles:"); strcat(s, circlesString); strcat(s, "\n");
	free(circlesString);

	char* pathsString = toString(g->paths);
	strcat(s, " paths:"); strcat(s, pathsString); strcat(s, "\n");
	free(pathsString);

	char* groupsString = toString(g->groups);
	strcat(s, " groups:"); strcat(s, groupsString); strcat(s, "\n");
	free(groupsString);

	char* otherAttributesStr = toString(g->otherAttributes);
	strcat(s, " otherAttributes:"); strcat(s, otherAttributesStr); strcat(s, "\n");
	free(otherAttributesStr);

	strcat(s,"↑end of group↑\n");
	return strdup(s);
}

int compareGroups(const void *first, const void *second) {
	return (1);
}

void deleteRectangle(void* data) {
	if (!data) return;

	Rectangle* r = (Rectangle*) data;

	freeList(r->otherAttributes);
	free(r);
}

char* rectangleToString(void* data) {
	if (!data) return (NULL);

	Rectangle* r = (Rectangle*) data;
	char s[5000]; s[0] = '\0';

	strcat(s,"\nRectangle:\n");

	char xstr[100];
	sprintf(xstr, "%.2f", r->x);
	strcat(s, " x coordinate: "); strcat(s, xstr); strcat(s, "\n");

	char ystr[100];
	sprintf(ystr, "%.2f", r->y);
	strcat(s, " y coordinate: "); strcat(s, ystr); strcat(s, "\n");

	char wstr[100];
	sprintf(wstr, "%.2f", r->width);
	strcat(s, " width: "); strcat(s, wstr); strcat(s, "\n");

	char hstr[100];
	sprintf(hstr, "%.2f", r->height);
	strcat(s, " height: "); strcat(s, hstr); strcat(s, "\n");

	strcat(s, " units: "); strcat(s, r->units); strcat(s, "\n");

	char* otherAttributesStr = toString(r->otherAttributes);
	strcat(s, " otherAttributes:"); strcat(s, otherAttributesStr); strcat(s, "\n");
	free(otherAttributesStr);

	return strdup(s);
}

int compareRectangles(const void *first, const void *second) {
	return (1);
}

void deleteCircle(void* data) {
	if (!data) return;

	Circle* c = (Circle *) data;

	freeList(c->otherAttributes);
	free(c);
}

char* circleToString(void* data) {
	if (!data) return (NULL);

	Circle* c = (Circle*) data;
	char s[5000]; s[0] = '\0';

	strcat(s,"\nCircle:\n");

	char cxStr[100];
	sprintf(cxStr, "%.2f", c->cx);
	strcat(s, " cx: "); strcat(s, cxStr); strcat(s, "\n");

	char cyStr[100];
	sprintf(cyStr, "%.2f", c->cy);
	strcat(s, " cy: "); strcat(s, cyStr); strcat(s, "\n");

	char rStr[100];
	sprintf(rStr, "%.2f", c->r);
	strcat(s, " r:"); strcat(s, rStr); strcat(s, "\n");

	strcat(s, " units: "); strcat(s, c->units); strcat(s, "\n");

	char* otherAttributesStr = toString(c->otherAttributes);
	strcat(s, " otherAttributes:"); strcat(s, otherAttributesStr); strcat(s, "\n");
	free(otherAttributesStr);

	return strdup(s);
}

int compareCircles(const void *first, const void *second) {
	return (1);
}

void deletePath(void* data) {
	if (!data) return;

	Path* p = (Path *) data;

	free(p->data);
	freeList(p->otherAttributes);
	free(p);
}

char* pathToString(void* data) {
	if (!data) return (NULL);

	Path* p = (Path*) data;
	char s[5000];
	s[0] = '\0';

	strcat(s,"\nPath:\n");

	strcat(s, "data: "); strcat(s, p->data); strcat(s, "\n");

	char* otherAttributesStr = toString(p->otherAttributes);
	strcat(s, " otherAttributes:"); strcat(s, otherAttributesStr); strcat(s, "\n");
	free(otherAttributesStr);

	return strdup(s);
}

int comparePaths(const void *first, const void *second) {
	return (1);
}

/* Helper Functions */
void parse_SVGimage(SVGimage* img, xmlNode* a_node) {
	xmlNode *cur_node = NULL;

	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if ( (strcmp((char*)cur_node->name, "title") == 0) && (img->title[0] == '\0') ) {
				strncpy(img->title, (char *) cur_node->children->content, 255);

			} else if ( (strcmp((char*)cur_node->name, "desc") == 0) && (img->description[0] == '\0') ) {
				strncpy(img->description, (char *) cur_node->children->content, 255);

			} else if (strcmp((char*)cur_node->name, "rect") == 0) {
				insertBack(img->rectangles, parse_rectangle(cur_node));

			} else if (strcmp((char*)cur_node->name, "circle") == 0) {
				insertBack(img->circles, parse_circle(cur_node));

			} else if (strcmp((char*)cur_node->name, "path") == 0) {
				insertBack(img->paths, parse_path(cur_node));

			} else if (strcmp((char*)cur_node->name, "g") == 0) {
				insertBack(img->groups, parse_group(cur_node));
			}
		}
	}

	parse_SVGImage_attributes(a_node, img);
}

SVGimage* SVGimage_Constructor(void) {
	SVGimage *img = malloc(sizeof(SVGimage));
	if (!img) return (NULL);

	img->title[0] = '\0';
	img->namespace[0] = '\0';
	img->description[0] = '\0';
	img->circles = initializeList(circleToString, deleteCircle, compareCircles);
	img->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
	img->paths = initializeList(pathToString, deletePath, comparePaths);
	img->groups = initializeList(groupToString, deleteGroup, compareGroups);
	img->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

	return (img);
}

char *strdup(const char *c) {
	char *dup = malloc(strlen(c) + 1);

	if (dup) strcpy(dup, c);

	return (dup);
}

void dummy_delete(void* d) {
}

Attribute* Attribute_Constructor(char* name, char* value) {
	if (!name || !value) return (NULL);

	Attribute* a = malloc(sizeof(Attribute));
	if (!a) return (NULL);

	a->name = NULL;
	a->value = NULL;

	a->name = malloc(sizeof(char) * (strlen(name) + 1));
	a->value = malloc(sizeof(char) * (strlen(value) + 1));

	strcpy(a->name, name);
	strcpy(a->value, value);

	return a;
}

Rectangle* Rectangle_Contructor() {
	Rectangle* r = malloc(sizeof(Rectangle));
	if (!r) return (NULL);

	r->x = 0;
	r->y = 0;
	r->height = 0;
	r->width = 0;
	r->units[0] = '\0';
	r->otherAttributes = initializeList(attributeToString,
										deleteAttribute,
										compareAttributes);

	return (r);
}

Circle* Circle_Constructor() {
	Circle*	c = malloc(sizeof(Circle));
	if (!c) return (NULL);

	c->cx = 0;
	c->cy = 0;
	c->r = 0;
	c->units[0] = '\0';
	c->otherAttributes = initializeList(attributeToString,
										deleteAttribute,
										compareAttributes);

	return(c);
}

Path* Path_Constructor() {
	Path* p = malloc(sizeof(Path));
	if (!p) return (NULL);

	p->data = NULL;
	p->otherAttributes = initializeList(attributeToString,
										deleteAttribute,
										compareAttributes);

	return (p);
}

Group* Group_Constructor() {
	Group* g = malloc(sizeof(Group));
	if (!g) return (NULL);

	g->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
	g->circles = initializeList(circleToString,	deleteCircle, compareCircles);
	g->paths = initializeList(pathToString, deletePath, comparePaths);
	g->groups = initializeList(groupToString, deleteGroup, compareGroups);
	g->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

	return (g);
}

void parse_SVGImage_attributes(xmlNode* node, SVGimage* img) {
	xmlAttr *attr;
	for (attr = node->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		Attribute* s = Attribute_Constructor(attrName, cont);
		insertBack(img->otherAttributes, s);
	}
}

void find_namespace(SVGimage* img, xmlNode* r) {
	if (!img || !r || !r->ns || !r->ns->href) return;

	char* ns = (char *) r->ns->href;
	strncpy(img->namespace, ns, 255);
}

Rectangle* parse_rectangle(xmlNode* node) {
	Rectangle* r = Rectangle_Contructor();

	xmlAttr *attr;
	for (attr = node->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		if (strcmp(attrName, "x") == 0) {
			r->x = strtof(cont, NULL);
			if (r->units[0] == '\0') sscanf(cont, "%*f%s", r->units);

		} else if (strcmp(attrName, "y") == 0) {
			r->y = strtof(cont, NULL);
			if (r->units[0] == '\0') sscanf(cont, "%*f%s", r->units);

		} else if (strcmp(attrName, "width") == 0) {
			r->width = strtof(cont, NULL);
			if (r->units[0] == '\0') sscanf(cont, "%*f%s", r->units);

		} else if (strcmp(attrName, "height") == 0) {
			r->height = strtof(cont, NULL);
			if (r->units[0] == '\0') sscanf(cont, "%*f%s", r->units);

		} else if (strcmp(attrName, "units") == 0) {
			strncpy(r->units, cont, 49);

		} else {
			Attribute* s = Attribute_Constructor(attrName, cont);
			insertBack(r->otherAttributes, s);
		}
	}

	return (r);
}

Circle* parse_circle(xmlNode* node) {
	Circle* c = Circle_Constructor();

	xmlAttr *attr;
	for (attr = node->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		if (strcmp(attrName, "cx") == 0) {
			c->cx = strtof(cont, NULL);
			if (c->units[0] == '\0') sscanf(cont, "%*f%s", c->units);

		} else if (strcmp(attrName, "cy") == 0) {
			c->cy = strtof(cont, NULL);
			if (c->units[0] == '\0') sscanf(cont, "%*f%s", c->units);

		} else if (strcmp(attrName, "r") == 0) {
			c->r = strtof(cont, NULL);
			if (c->units[0] == '\0') sscanf(cont, "%*f%s", c->units);

		} else if (strcmp(attrName, "units") == 0) {
			strncpy(c->units, cont, 49);

 		} else {
			Attribute* s = Attribute_Constructor(attrName, cont);
			insertBack(c->otherAttributes, s);
		}
	}

	return (c);
}

Path* parse_path(xmlNode* node) {
	Path* p = Path_Constructor();

	xmlAttr *attr;
	for (attr = node->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		if (strcmp(attrName, "d") == 0) {
			p->data = malloc(sizeof(char) * (strlen(cont)+1));
			strcpy(p->data, cont);
		} else {
			Attribute* s = Attribute_Constructor(attrName, cont);
			insertBack(p->otherAttributes, s);
		}
	}

	return (p);
}

Group* parse_group(xmlNode* a_node) {
	Group* g = Group_Constructor();
	xmlNode *cur_node = NULL;

	for (cur_node = a_node->children; cur_node != NULL; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (strcmp((char*)cur_node->name, "rect") == 0) {
				insertBack(g->rectangles, parse_rectangle(cur_node));

			} else if (strcmp((char*)cur_node->name, "circle") == 0) {
				insertBack(g->circles, parse_circle(cur_node));

			} else if (strcmp((char*)cur_node->name, "path") == 0) {
				insertBack(g->paths, parse_path(cur_node));

			} else if ((strcmp((char*)cur_node->name, "g") == 0) && (cur_node->children)) {
				insertBack(g->groups, parse_group(cur_node)); //recursion
			}
		}
	}

	xmlAttr *attr;
	for (attr = a_node->properties; attr != NULL; attr = attr->next) {
		xmlNode *value = attr->children;
		char *attrName = (char *)attr->name;
		char *cont = (char *)(value->content);

		Attribute* s = Attribute_Constructor(attrName, cont);
		insertBack(g->otherAttributes, s);
	}

	return (g);
}

void recursive_rectangles(List* l, Group* g) {
	if (!l || !g) return;

	ListIterator I = createIterator(g->rectangles);
	Rectangle* ptr;
	while((ptr = (Rectangle*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(g->groups);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_rectangles(l, gPtr);
	}
}

void recursive_circles(List* l, Group* g) {
	if (!l || !g) return;

	ListIterator I = createIterator(g->circles);
	Circle* ptr;
	while((ptr = (Circle*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(g->groups);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_circles(l, gPtr);
	}
}

void recursive_groups(List* l, Group* g) {
	if (!l || !g) return;

	ListIterator I = createIterator(g->groups);
	Group* ptr;
	while((ptr = (Group*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
		recursive_groups(l, ptr);
	}
}

void recursive_paths(List* l, Group* g) {
	if (!l || !g) return;

	ListIterator I = createIterator(g->paths);
	Path* ptr;
	while((ptr = (Path*)nextElement(&I)) != NULL) {
		insertBack(l, ptr);
	}

	I = createIterator(g->groups);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&I)) != NULL) {
		recursive_paths(l, gPtr);
	}
}

bool assign_xml_svg_basic(SVGimage* img, xmlNode* r) {
	if (img->namespace[0] == '\0') return (false);
	xmlSetNs(r, xmlNewNs(r, (xmlChar*) img->namespace, NULL));

	if ((img->title) && (img->title[0] != '\0')) {
		xmlNewChild(r, NULL, (const xmlChar*) "title", (const xmlChar*) img->title);
	}

	if ((img->description) && (img->description[0] != '\0')) {
		xmlNewChild(r, NULL, (const xmlChar*) "desc", (const xmlChar*) img->description);
	}

	if (!img->otherAttributes) return (false);
	ListIterator aI = createIterator(img->otherAttributes);
	Attribute* aPtr;
	while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
		if ((aPtr->name) && (aPtr->value)) {
			xmlNewProp(r, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
		} else return (false);
	}

	return (true);
}

bool assign_xml_svg_rectangles(SVGimage* img, xmlNode* r) {
	char buffer[500];

	if (!img->rectangles) return (false);
	ListIterator rI = createIterator(img->rectangles);
	Rectangle* rPtr;
	while((rPtr = (Rectangle*)nextElement(&rI)) != NULL) {
		xmlNode* rec = xmlNewChild(r, NULL, (xmlChar*)"rect", NULL);

		sprintf(buffer, "%f%s", rPtr->x, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"x", (xmlChar*) buffer);

		sprintf(buffer, "%f%s", rPtr->y, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"y", (xmlChar*) buffer);

		if (rPtr->width < 0) return (false);
		sprintf(buffer, "%f%s", rPtr->width, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"width", (xmlChar*) buffer);

		if (rPtr->height < 0) return (false);
		sprintf(buffer, "%f%s", rPtr->height, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"height", (xmlChar*) buffer);

		if (!rPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(rPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(rec, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	return (true);
}

bool assign_xml_svg_circles(SVGimage* img, xmlNode* r) {
	char buffer[500];

	if (!img->circles) return (false);
	ListIterator cI = createIterator(img->circles);
	Circle* cPtr;
	while((cPtr = (Circle*)nextElement(&cI)) != NULL) {
		xmlNode* circle = xmlNewChild(r, NULL, (xmlChar*)"circle", NULL);

		sprintf(buffer, "%f%s", cPtr->cx, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"cx", (xmlChar*)buffer);

		sprintf(buffer, "%f%s", cPtr->cy, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"cy", (xmlChar*)buffer);

		if(cPtr->r < 0) return (false);
		sprintf(buffer, "%f%s", cPtr->r, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"r", (xmlChar*)buffer);

		if (!cPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(cPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(circle, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	return (true);
}

bool assign_xml_svg_paths(SVGimage* img, xmlNode* r) {
	if (!img->paths) return (false);
	ListIterator pI = createIterator(img->paths);
	Path* pPtr;
	while((pPtr = (Path*)nextElement(&pI)) != NULL) {
		xmlNode* path = xmlNewChild(r, NULL, (xmlChar*)"path", NULL);

		if (!pPtr->data) return (false);
		xmlNewProp(path, (xmlChar*)"d", (xmlChar*)pPtr->data);

		if (!pPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(pPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(path, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	return (true);
}

bool assign_xml_svg_groups(SVGimage* img, xmlNode* r) {
	if (!img->groups) return (false);
	ListIterator gI = createIterator(img->groups);
	Group* gPtr;
	while((gPtr = (Group*)nextElement(&gI)) != NULL) {
		xmlNode* childGroup = xmlNewChild(r, NULL, (xmlChar*) "g", NULL);

		if (!assign_xml_recursive_groups(gPtr, childGroup)) return (false);
	}

	return (true);
}

bool assign_xml_recursive_groups(Group* g, xmlNode* groupNode) {
	char buffer[3000];

	/* Rectangles */
	if (!g->rectangles) return (false);
	ListIterator rI = createIterator(g->rectangles);
	Rectangle* rPtr;
	while((rPtr = (Rectangle*)nextElement(&rI)) != NULL) {
		xmlNode* rec = xmlNewChild(groupNode, NULL, (xmlChar*)"rect", NULL);

		sprintf(buffer, "%f%s", rPtr->x, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"x", (xmlChar*) buffer);

		sprintf(buffer, "%f%s", rPtr->y, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"y", (xmlChar*) buffer);

		if (rPtr->width < 0) return (false);
		sprintf(buffer, "%f%s", rPtr->width, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"width", (xmlChar*) buffer);

		if (rPtr->height < 0) return (false);
		sprintf(buffer, "%f%s", rPtr->height, rPtr->units);
		xmlNewProp(rec, (xmlChar*)"height", (xmlChar*) buffer);

		if (!rPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(rPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(rec, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	/* Circles */
	if (!g->circles) return (false);
	ListIterator cI = createIterator(g->circles);
	Circle* cPtr;
	while((cPtr = (Circle*)nextElement(&cI)) != NULL) {
		xmlNode* circle = xmlNewChild(groupNode, NULL, (xmlChar*)"circle", NULL);

		sprintf(buffer, "%f%s", cPtr->cx, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"cx", (xmlChar*)buffer);

		sprintf(buffer, "%f%s", cPtr->cy, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"cy", (xmlChar*)buffer);

		if(cPtr->r < 0) return (false);
		sprintf(buffer, "%f%s", cPtr->r, cPtr->units);
		xmlNewProp(circle, (xmlChar*)"r", (xmlChar*)buffer);

		if (!cPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(cPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(circle, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	/* Paths */
	if (!g->paths) return (false);
	ListIterator pI = createIterator(g->paths);
	Path* pPtr;
	while((pPtr = (Path*)nextElement(&pI)) != NULL) {
		xmlNode* path = xmlNewChild(groupNode, NULL, (xmlChar*)"path", NULL);

		if (pPtr->data) {
			xmlNewProp(path, (xmlChar*)"d", (xmlChar*)pPtr->data);
		}

		if (!pPtr->otherAttributes) return (false);
		ListIterator aI = createIterator(pPtr->otherAttributes);
		Attribute* aPtr;
		while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
			if ((aPtr->name) && (aPtr->value)) {
				xmlNewProp(path, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
			} else return (false);
		}
	}

	/* Other attributes */
	if (!g->otherAttributes) return (false);
	ListIterator aI = createIterator(g->otherAttributes);
	Attribute* aPtr;
	while((aPtr = (Attribute*)nextElement(&aI)) != NULL) {
		if ((aPtr->name) && (aPtr->value)) {
			xmlNewProp(groupNode, (xmlChar*)aPtr->name, (xmlChar*)aPtr->value);
		} else return (false);
	}

	/* Groups */
	if (!g->groups) return (false);
	ListIterator grI = createIterator(g->groups);
	Group* grPtr;
	while((grPtr = (Group*)nextElement(&grI)) != NULL) {
		xmlNode* newGroup = xmlNewChild(groupNode, NULL, (xmlChar*) "g", NULL);
		if (!assign_xml_recursive_groups(grPtr, newGroup)) return (false);
	}

	return (true);
}

xmlDoc* create_SVG_Tree(SVGimage* image) {
	if (!image) return (false);

	xmlDoc* doc = xmlNewDoc((xmlChar*) "1.0");
	if (!doc) return (false);

	xmlNode* root = xmlNewNode(NULL, (xmlChar *) "svg");
	if (!root) return (false);

	xmlDocSetRootElement(doc, root);

	/* making nodes */
	if (
		!assign_xml_svg_basic(image, root) ||
		!assign_xml_svg_rectangles(image, root) ||
		!assign_xml_svg_circles(image, root) ||
		!assign_xml_svg_paths(image, root) ||
		!assign_xml_svg_groups(image, root)
	) {
		xmlFreeDoc(doc);
		return (NULL);
	}

	return (doc);
}

bool validate_xml(xmlDoc* doc, char* schemaFile) {
	if (!doc || !schemaFile) return (false);

	xmlLineNumbersDefault(1);

	xmlSchemaParserCtxtPtr ctxt = NULL;
	ctxt = xmlSchemaNewParserCtxt(schemaFile);
	if (!ctxt) return (false);

	xmlSchemaPtr schema = NULL;
	schema = xmlSchemaParse(ctxt);
	xmlSchemaSetParserErrors(ctxt,
							 (xmlSchemaValidityErrorFunc) fprintf,
							 (xmlSchemaValidityWarningFunc) fprintf,
							 stderr);

	if (!schema) return (false);
	xmlSchemaFreeParserCtxt(ctxt);

	xmlSchemaValidCtxtPtr vctxt = NULL;
	vctxt = xmlSchemaNewValidCtxt(schema);
	if (!vctxt) return (false);
	xmlSchemaSetValidErrors(vctxt,
							(xmlSchemaValidityErrorFunc) fprintf,
							(xmlSchemaValidityWarningFunc) fprintf,
							stderr);

	int ret;
	ret = xmlSchemaValidateDoc(vctxt, doc);

	xmlSchemaFreeValidCtxt(vctxt);
	xmlFreeDoc(doc);
	if (schema) xmlSchemaFree(schema);
	xmlSchemaCleanupTypes();
	xmlCleanupParser();

	return (ret == 0);
}