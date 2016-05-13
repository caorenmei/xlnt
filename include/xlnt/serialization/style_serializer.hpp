// Copyright (c) 2014-2016 Thomas Fussell
// Copyright (c) 2010-2015 openpyxl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, WRISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <xlnt/xlnt_config.hpp>
#include <xlnt/styles/named_style.hpp>
#include <xlnt/workbook/workbook.hpp>

namespace xlnt {

class alignment;
class border;
class cell_style;
class color;
class common_style;
class conditional_format;
class fill;
class font;
class format;
class named_style;
class number_format;
class protection;
class side;
class xml_document;
class xml_node;

/// <summary>
/// Reads and writes xl/styles.xml from/to an associated workbook.
/// </summary>
class XLNT_CLASS style_serializer
{
public:
    /// <summary>
    /// Construct a style_serializer which can write styles.xml based on wb or populate wb
    /// with styles from an existing styles.xml.
    style_serializer(workbook &wb);

    //
    // Primary methods
    //

    /// <summary>
    /// Load all styles from xml_document into workbook given in constructor.
    /// </summary>
    bool read_stylesheet(const xml_document &xml);

    /// <summary>
    /// Populate parameter xml with an XML tree representing the styles contained in the workbook
    /// given in the constructor.
    /// </summary>
    xml_document write_stylesheet();

    // TODO: These need to be public for unit tests. Could also make test class a friend?
    // private:

    //
    // Static element readers (i.e. readers that don't use internal state)
    //

    /// <summary>
    /// Read and return an alignment from alignment_node.
    /// </summary>
    static alignment read_alignment(const xml_node &alignment_node);

    /// <summary>
    /// Read and return a border side from side_node.
    /// </summary>
    static side read_side(const xml_node &side_node);

    /// <summary>
    /// Read and return a border from border_node.
    /// </summary>
    static border read_border(const xml_node &border_node);

    /// <summary>
    /// Read and return a fill from fill_node.
    /// </summary>
    static fill read_fill(const xml_node &fill_node);

    /// <summary>
    /// Read and return a font from font_node.
    /// </summary>
    static font read_font(const xml_node &font_node);

    /// <summary>
    /// Read and return a number format from number_format_node.
    /// </summary>
    static number_format read_number_format(const xml_node &number_format_node);

    /// <summary>
    /// Read and return a protection from protection_node.
    /// </summary>
    static protection read_protection(const xml_node &protection_node);

    /// <summary>
    /// Read and return all indexed colors from indexed_colors_node.
    /// </summary>
    static std::vector<color> read_indexed_colors(const xml_node &indexed_colors_node);

    /// <summary>
    /// Read and return a color from color_node.
    /// </summary>
    static color read_color(const xml_node &color_node);

    /// <summary>
    /// Read and return a conditional format, dxf, from conditional_format_node.
    /// </summary>
    static conditional_format read_conditional_format(const xml_node &conditional_formats_node);

    /// <summary>
    /// Read a single format from the given node. In styles.xml, this is an "xf" element.
    /// A format has an optional border id, fill id, font id, and number format id where
    /// each id is an index in the corresponding list of borders, etc. A style also has
    /// optional alignment and protection children. A style also defines whether each of
    /// these is "applied". For example, a style with a defined font id, font=# but with
    /// "applyFont=0" will not use the font in formatting.
    /// </summary>
    cell_style read_cell_style(const xml_node &style_node);
    
    /// <summary>
    /// Read a single named style from the given named_style_node. In styles.xml, this is a
    /// (confusingly named) "cellStyle" element. This node defines the name, whether it is
    /// built-in and an xfId which is the index of an element in cellStyleXfs which should
    /// be provided as the style_node parameter (cellStyleXfs itself, not the child xf node).
    /// </summary>
    named_style read_named_style(const xml_node &named_style_node, const xml_node &style_parent_node);
    
    //
    // Non-static element readers (i.e. readers that modify internal state)
    //
    
    bool read_number_formats(const xml_node &number_formats_node);

    /// <summary>
    /// Read all borders from borders_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_borders(const xml_node &borders_node);

    /// <summary>
    /// Read all fills from fills_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_fills(const xml_node &fills_node);

    /// <summary>
    /// Read all fonts from fonts_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_fonts(const xml_node &fonts_node);

    /// <summary>
    /// Read all colors from colors_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_colors(const xml_node &colors_node);
    
    /// <summary>
    /// Read all cell styles from cell_styles_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_cell_styles(const xml_node &cell_styles_node);

    /// <summary>
    /// Read all named styles from named_style_node and cell_styles_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_named_styles(const xml_node &named_styles_node, const xml_node &cell_styles_node);

    /// <summary>
    /// Read all borders from number_formats_node and add them to workbook.
    /// Return true on success.
    /// </summary>
    bool read_stylesheet(const xml_node &number_formats_node);

    //
    // Non-static element writers (i.e. writers that modify internal workbook)
    //

    /// <summary>
    /// Build an xml tree representing all borders in workbook into borders_node.
    /// Returns true on success.
    /// </summary>
    bool write_borders(xml_node &borders_node) const;

    /// <summary>
    /// Build an xml tree representing all conditional formats in workbook into conditional_formats_node.
    /// Returns true on success.
    /// </summary>
    bool write_conditional_formats(xml_node &conditional_formats_node) const;

    /// <summary>
    /// Build an xml tree representing all fills in workbook into fills_node.
    /// Returns true on success.
    /// </summary>
    bool write_fills(xml_node &fills_node) const;

    /// <summary>
    /// Build an xml tree representing all fonts in workbook into fonts_node.
    /// Returns true on success.
    /// </summary>
    bool write_fonts(xml_node &fonts_node) const;

    /// <summary>
    /// Build an xml tree representing all number formats in workbook into number_formats_node.
    /// Returns true on success.
    /// </summary>
    bool write_number_formats(xml_node &number_formats_node) const;

    bool write_style_common(const common_style &style, xml_node xf_node) const;

    /// <summary>
    /// Build an xml tree representing the given style into style_node.
    /// Returns true on success.
    /// </summary>
    bool write_cell_style(const cell_style &style, xml_node &style_node) const;
    
    /// <summary>
    /// Build an xml tree representing the given style into style_node.
    /// Returns true on success.
    /// </summary>
    bool write_cell_styles(xml_node &styles_node) const;
    
    /// <summary>
    /// Build an xml tree representing the given style into cell_styles_node and styles_node.
    /// Returns true on success.
    /// </summary>
    bool write_named_style(const named_style &style, xml_node &cell_styles_node) const;
    
    bool write_named_styles(xml_node &cell_styles_node, xml_node &styles_node) const;
    
    bool write_colors(xml_node &colors_node) const;
    
    bool write_ext_list(xml_node &ext_list_node) const;
    
    const std::vector<border> &get_borders() const;
    
    const std::vector<fill> &get_fills() const;
    
    const std::vector<font> &get_fonts() const;
    
    const std::vector<number_format> &get_number_formats() const;
    
    const std::vector<color> &get_colors() const;
    
private:
    void initialize_vectors();
    
    /// <summary>
    /// Set in the constructor, this workbook is used as the source or target for all writing or reading, respectively.
    /// </summary>
    workbook &workbook_;
    
    std::vector<color> colors_;
    std::vector<border> borders_;
    std::vector<fill> fills_;
    std::vector<font> fonts_;
    std::vector<number_format> number_formats_;
    std::vector<cell_style> cell_styles_;
    std::unordered_map<std::size_t, named_style> named_styles_;
};

} // namespace xlnt
