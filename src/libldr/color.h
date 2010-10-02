/* libLDR: Portable and easy-to-use LDraw format abstraction & I/O reference library *
 * To obtain more information about LDraw, visit http://www.ldraw.org.               *
 * Distributed in terms of the GNU Lesser General Public License v3                  *
 *                                                                                   *
 * Author: (c)2006-2008 Park "segfault" J. K. <mastermind_at_planetmono_dot_org>     */

#ifndef _LIBLDR_COLOR_H_
#define _LIBLDR_COLOR_H_

#include <map>
#include <string>

#include "common.h"

namespace ldraw
{

enum material_type {
	material_normal,
	material_transparent,
	material_metallic,
	material_rubber,
	material_phosphorus
};

struct color_entity
{
	material_type material;
	unsigned char rgba[4]; // RGBA Array
	int id1; // LDraw Color ID
	int id2; // Alternate Color ID
	std::string name; // Name String
};

// Represents a color.
class LIBLDR_EXPORT color
{
  public:
	static const color_entity color_chart[];
	static const std::map<int, const color_entity *> color_map;

	static void init();
	
	color() : m_valid(true), m_id(0) { link(); }
	color(int id) : m_id(id) { link();}
	color(const color &c) : m_id(c.get_id()) { link(); }
	~color();
	
	void operator=(int rhs) { m_id = rhs; link(); }
	void operator=(const color &rhs) { m_id = rhs.get_id(); link(); }
	
	int get_id() const { return m_id; }
	void set_id(int i) { m_id = i; link(); }
	
	bool is_valid() { return m_valid; }
	bool is_null() { return m_id == 16 || m_id == 24; }
	const color_entity* get_entity() const { return m_entity; }
	static const color_entity* get_complement(int id);
	const color_entity* get_complement() const { return get_complement(m_id); }
	
  private:
	static bool m_initialized;
	
	void link();
	
	bool m_custom_color;
	bool m_valid;
	int m_id;
	const color_entity *m_entity;
};

}

#endif
