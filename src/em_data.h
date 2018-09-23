/***************************************************************
 * Name:      em_data.h
 * Purpose:   Internal representation of Extended M3U files
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-09
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#ifndef EM_DATA_H
#define EM_DATA_H

#include <vector>
#include <memory>
#include <optional>
#include <utility>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/crt.h>

// Custom attribute
class EMAttr final
{
public:
    explicit EMAttr(wxString s);
    wxString toString() const;
    void normalize() { setDelimiter(' '); setQuoted(true); }

    wxString getKey() const { return m_key; }
    void setKey(wxString key) { wxASSERT(!key.IsEmpty()); m_key = std::move(key); }

    bool hasValue() const { return m_val.has_value(); }
    wxString getValue() const { return m_val.value_or("<NO_VALUE>"); }
    void setValue(wxString value);
    void deleteValue() { m_val.reset(); }

    // Delimiter before this attribute
    wxString getDelimiter() const { return m_delim; }
    void setDelimiter(wxString d) { m_delim = std::move(d); }

    // Quotes around the value
    bool getQuoted() const { return m_quoted; }
    void setQuoted(bool quoted);

    static bool isDelimChar(wxUniChar c) { return wxIsspace(c) || c == ','; }

private:
    bool hasDelimiterInValue() const;

    wxString m_delim;
    wxString m_key;
    std::optional<wxString> m_val;
    bool m_quoted  = true;
};

// Abstract line
class EMLine
{
public:
    virtual wxString toString() const =0;
    virtual void normalize() {}
    virtual bool isHeader() const { return false; }
    virtual bool isEmpty() const { return false; }
    virtual ~EMLine() {}

    static std::unique_ptr<EMLine> parse(const wxString& line);
};

// Generic unstructured line (commment, empty line, etc)
class EMLineText : public EMLine
{
public:
    explicit EMLineText(wxString val) : m_val(std::move(val)) {}
    wxString toString() const override { return m_val; }

    bool isEmpty() const override { return m_val.IsEmpty(); }
    wxString getText() const { return m_val; }
    void setText(wxString text) { m_val = std::move(text); }

protected:
    wxString m_val;
};

// URL line
class EMLineURL final : public EMLineText
{
public:
    explicit EMLineURL(wxString url) : EMLineText(std::move(url)) {}
    void normalize() override { m_val.Trim().Trim(false); }
};

// Abstract directive line
class EMLineDirectiveBase : public EMLine
{
public:
    wxString toString() const override;
    void normalize() override;

    bool hasAttributes() const { return !m_attr.empty(); }
    auto& getAttributes() { return m_attr; }

    virtual wxString getDirective() const =0;

    // Delimiter after the attributes
    wxString getAfterAttr() const { return m_after_attr; }
    void setAfterAttr(wxString aa) { m_after_attr = std::move(aa); }

protected:
    virtual wxString attrString() const;
    void parseAttr(const wxString& attr);

    std::vector<EMAttr> m_attr;
    wxString m_after_attr;
};

// Generic directive line
class EMLineDirective final : public EMLineDirectiveBase
{
public:
    explicit EMLineDirective(const wxString& l);

    wxString getDirective() const override { return m_dir; }
    void setDirective(wxString dir);
    bool isHeader() const override { return m_dir == "EXTM3U"; }

private:
    static bool checkDirective(const wxString& dir);

    wxString m_dir;
};

// #EXTINF directive line
class EMLineExtinf final : public EMLineDirectiveBase
{
public:
    explicit EMLineExtinf(const wxString& l);
    void normalize() override;

    wxString getDirective() const override { return "EXTINF:"; }

    // Delimiter before the length
    wxString getPreLen() const { return m_pre_len; }
    void setPreLen(wxString pl) { m_pre_len = std::move(pl); }

    bool hasLen() const { return m_len.has_value(); }
    int getLen() const { return m_len.value(); }
    wxString getLenStr() const { return (hasLen()? wxString()<<getLen() : "<NO_VALUE>"); }
    void setLen(int len);

    bool hasName() const { return !m_name.IsEmpty(); }
    wxString getName() const { return m_name; }
    void setName(wxString name);

private:
    wxString attrString() const override;

    wxString m_pre_len;
    std::optional<int> m_len;
    wxString m_name;
};

// Playlist entry
class EMEntry
{
public:
    explicit EMEntry(std::vector<std::unique_ptr<EMLine>> lines);
    wxArrayString toText() const;
    void normalize() { for (auto& l : m_lines) l->normalize(); }

    auto& getLines() const { return m_lines; }

    bool hasExtinf() { return m_extinf; }
    auto& getExtinf() { return *m_extinf; }

    bool hasURL() { return m_url; }
    auto& getURL() { return *m_url; }

private:
    std::vector<std::unique_ptr<EMLine>> m_lines;
    EMLineExtinf* m_extinf = nullptr;
    EMLineURL* m_url = nullptr;
};

// M3U file content
class EMFileData
{
public:
    explicit EMFileData(const wxArrayString& text);
    wxArrayString toText() const;
    void normalize();

    wxArrayString headText() const;
    wxArrayString endText() const;

    auto& getHeader() { return m_head; }
    auto& getEntries() { return m_entries; }
    auto& getEnding() { return m_end; }

    // Common delimiter (empty lines only)
    unsigned char getCommonDelimSize() const { return m_delim_n; }
    void setCommonDelimSize(unsigned char dn) { m_delim_n = dn; }

    // Common delimiter after the last entry
    bool getCommonDelimAtEnd() const { return m_delim_end; }
    void setCommonDelimAtEnd(bool de) { m_delim_end = de; }

private:

    std::vector<std::unique_ptr<EMLine>> m_head;
    std::vector<EMEntry> m_entries;
    std::vector<std::unique_ptr<EMLine>> m_end;
    unsigned char m_delim_n = 0;
    bool m_delim_end = false;
};

#endif // EM_DATA_H
