/***************************************************************
 * Name:      tests_data.cpp
 * Purpose:   Unit tests for ExtM3U internal representation
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-20
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "catch.hpp"
#include "../src/em_data.h"
#include <wx/log.h>
#include <typeinfo>

TEST_CASE("Attribute parsing", "[parse]")
{
    SECTION("Simple attribute without value")
    {
        EMAttr a(" key");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK_FALSE(a.hasValue());
    }

    SECTION("Simple attribute with value")
    {
        EMAttr a(" key=\"val\"");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "val");
    }

    SECTION("Attribute with unquoted value")
    {
        EMAttr a(" key=val");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK_FALSE(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "val");
    }

    SECTION("Attribute with unquoted number value")
    {
        EMAttr a(" key=123");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK_FALSE(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "123");
    }

    SECTION("Attribute with empty value")
    {
        EMAttr a(" key=\"\"");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "");
    }

    SECTION("Attribute with empty unquoted value")
    {
        EMAttr a(" key=");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK_FALSE(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "");
    }

    SECTION("Attribute with empty delimiter")
    {
        EMAttr a("key=\"val\"");
        CHECK(a.getDelimiter() == "");
        CHECK(a.getKey() == "key");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "val");
    }

    SECTION("Attribute with complex delimiter")
    {
        EMAttr a("\t, key=\"val\"");
        CHECK(a.getDelimiter() == "\t, ");
        CHECK(a.getKey() == "key");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "val");
    }

    SECTION("Attribute with complex key")
    {
        EMAttr a(" Some_complex:#$*_key123=\"val\"");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "Some_complex:#$*_key123");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == "val");
    }

    SECTION("Attribute with complex value")
    {
        wxString val = " This is: long, complex value... \t "
            "with {/:\\@*#}'^&>[!]?# (special symbols) "
            "and 123 3.14 1/2 3:4 (numbers)";
        EMAttr a(" key=\"" + val + "\"");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == val);
    }

    SECTION("Attribute with complex unquoted value")
    {
        wxString val = "Long_value_without_delimiters..."
            "but_with_numbers:_123_3.14_1/2_3:4;"
            "and_special_symbols:_!@#$%^&*()-=+/'\\";
        EMAttr a(" key=" + val);
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getKey() == "key");
        CHECK_FALSE(a.getQuoted());
        REQUIRE(a.hasValue());
        CHECK(a.getValue() == val);
    }
}

TEST_CASE("Attribute editing and generating", "[edit][generate]")
{
    EMAttr a("a");
    REQUIRE(a.getDelimiter() == "");
    REQUIRE(a.getKey() == "a");
    REQUIRE_FALSE(a.hasValue());
    REQUIRE(a.toString() == "a");

    SECTION("Adding delimiter")
    {
        a.setDelimiter(" ");
        CHECK(a.getDelimiter() == " ");
        CHECK(a.toString() == " a");

        // Editing delimiter
        a.setDelimiter(", ");
        CHECK(a.getDelimiter() == ", ");
        CHECK(a.toString() == ", a");
    }

    SECTION("Editing key")
    {
        a.setKey("New_key");
        CHECK(a.getKey() == "New_key");
        CHECK(a.toString() == "New_key");
    }

    SECTION("Adding value")
    {
        a.setValue("value");
        REQUIRE(a.hasValue());
        REQUIRE(a.getValue() == "value");
        REQUIRE(a.toString() == "a=\"value\"");

        SECTION("Editing value")
        {
            a.setValue("New value");
            REQUIRE(a.hasValue());
            CHECK(a.getValue() == "New value");
            CHECK(a.toString() == "a=\"New value\"");
        }

        SECTION("Removing quotes")
        {
            a.setQuoted(false);
            REQUIRE_FALSE(a.getQuoted());
            REQUIRE(a.toString() == "a=value");

            SECTION("Adding quotes")
            {
                a.setQuoted(true);
                REQUIRE(a.getQuoted());
                CHECK(a.toString() == "a=\"value\"");
            }

            SECTION("Setting value with delimiters")
            {
                auto ll = wxLog::GetLogLevel();
                wxLog::SetLogLevel(wxLOG_Error); // Suppress warning message
                a.setValue("new value");
                wxLog::SetLogLevel(ll);
                REQUIRE(a.getQuoted());
                CHECK(a.toString() == "a=\"new value\"");
            }
        }

        SECTION("Removing value")
        {
            a.deleteValue();
            CHECK_FALSE(a.hasValue());
            CHECK(a.toString() == "a");
        }
    }
}

TEST_CASE("Attribute normalization", "[normalize]")
{
    auto CheckNormalize = [](auto& s)
    {
        INFO(s);
        EMAttr a(s);
        a.normalize();
        CHECK(a.getDelimiter() == " ");
        CHECK(a.getQuoted());
        CHECK(a.toString() == " key=\"val\"");
    };
    CheckNormalize(" key=\"val\"");
    CheckNormalize(" key=val");
    CheckNormalize("key=\"val\"");
    CheckNormalize("key=val");
    CheckNormalize(" \t, key=\"val\"");
    CheckNormalize(" \t, key=val");
}

TEST_CASE("Text line", "[edit]")
{
    EMLineText l("#qwerty");
    CHECK_FALSE(l.isEmpty());
    CHECK_FALSE(l.isHeader());
    CHECK(l.getText() == "#qwerty");
    CHECK(l.toString() == "#qwerty");

    l.setText("");
    CHECK(l.isEmpty());
    CHECK_FALSE(l.isHeader());
    CHECK(l.getText() == "");
    CHECK(l.toString() == "");
}

TEST_CASE("URL line", "[edit][normalize]")
{
    EMLineURL l("http://site.com/file.avi");

    SECTION("Normal URL")
    {
        CHECK(l.getText() == "http://site.com/file.avi");
        CHECK(l.toString() == "http://site.com/file.avi");
    }

    SECTION("URL with whitespace")
    {
        l.setText("\tC:\\some folder\\file.mp3 ");

        SECTION("Non-normalized")
        {
            CHECK(l.getText() == "\tC:\\some folder\\file.mp3 ");
            CHECK(l.toString() == "\tC:\\some folder\\file.mp3 ");
        }

        SECTION("Normalized")
        {
            l.normalize();
            CHECK(l.getText() == "C:\\some folder\\file.mp3");
            CHECK(l.toString() == "C:\\some folder\\file.mp3");
        }
    }

    CHECK_FALSE(l.isEmpty());
    CHECK_FALSE(l.isHeader());
}

TEST_CASE("Parsing directive", "[parse]")
{
    wxArrayString attrs;
    wxString d = "SOMEDIR";
    auto CheckDirective = [&]
    {
        wxString s = '#' + d;
        for(auto& a : attrs)
            s.append(a);
        SECTION("Without whitespace") {}
        SECTION("With whitespace") { s.append("  "); }
        INFO(s);
        EMLineDirective l(s);
        CHECK(l.getDirective() == d);
        CHECK(l.toString() == s);
        REQUIRE(l.getAttributes().size() == attrs.size());
        for(std::size_t i=0; i<attrs.size(); ++i)
            CHECK(l.getAttributes()[i].toString() == attrs[i]);
    };

    CheckDirective();
    attrs.emplace_back(" k1=\"v1\"");
    CheckDirective();
    attrs.emplace_back(" k2=\"v 2\"");
    CheckDirective();
    attrs.emplace_back(" k3=v3");
    CheckDirective();
    attrs.emplace_back(" ,\t k4=v4");
    CheckDirective();
    attrs.emplace_back(",k5=\"v, #$#@'://!-= 5\"");
    CheckDirective();
    d.append(':');
    CheckDirective();
    attrs.insert(attrs.begin(), "k0=v0");
    CheckDirective();
}

TEST_CASE("Editing and generating directive", "[edit][generate]")
{
    EMLineDirective l("#DIR");
    REQUIRE(l.getDirective() == "DIR");
    REQUIRE_FALSE(l.isHeader());
    REQUIRE_FALSE(l.hasAttributes());
    REQUIRE(l.toString() == "#DIR");

    SECTION("Editing directive")
    {
        l.setDirective("EXTM3U");
        CHECK(l.isHeader());
        CHECK_FALSE(l.hasAttributes());
        CHECK(l.getDirective() == "EXTM3U");
        CHECK(l.toString() == "#EXTM3U");
    }

    SECTION("Adding attributes")
    {
        l.getAttributes().emplace_back(" key=\"val\"");
        CHECK(l.hasAttributes());
        CHECK(l.toString() == "#DIR key=\"val\"");
    }
}

TEST_CASE("Normalizing directive", "[normalize]")
{
    EMLineDirective l1("#DIR ");
    l1.normalize();
    CHECK(l1.toString() == "#DIR");

    EMLineDirective l2("#DIR  k1=v1, k2=\"v 2\"\tk3   ");
    l2.normalize();
    CHECK(l2.toString() == "#DIR k1=\"v1\" k2=\"v 2\" k3");
}

TEST_CASE("Parsing EXTINF", "[parse]")
{
    wxString d = "#EXTINF:";

    SECTION("Empty")
    {
        EMLineExtinf l(d);
        CHECK_FALSE(l.hasAttributes());
        CHECK_FALSE(l.hasLen());
        CHECK_FALSE(l.hasName());
        CHECK(l.getPreLen() == "");
        CHECK(l.getAfterAttr() == "");
        CHECK(l.toString() == d);
    }

    SECTION("With length")
    {
        wxString s = d + "123";
        EMLineExtinf l(s);
        CHECK_FALSE(l.hasAttributes());
        CHECK_FALSE(l.hasName());
        CHECK(l.getPreLen() == "");
        CHECK(l.getAfterAttr() == "");
        CHECK(l.toString() == s);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == 123);
    }

    SECTION("With length and name")
    {
        wxString s = d + "123,Name 1";
        EMLineExtinf l(s);
        CHECK_FALSE(l.hasAttributes());
        CHECK(l.hasName());
        CHECK(l.getName() == "Name 1");
        CHECK(l.getPreLen() == "");
        CHECK(l.getAfterAttr() == ",");
        CHECK(l.toString() == s);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == 123);
    }

    SECTION("With length, attributes and name")
    {
        wxString s = d + "-1 k1=v1 k2=\"v, 2\",Name 2";
        EMLineExtinf l(s);
        CHECK(l.hasAttributes());
        CHECK(l.hasName());
        CHECK(l.getName() == "Name 2");
        CHECK(l.getPreLen() == "");
        CHECK(l.getAfterAttr() == ",");
        CHECK(l.toString() == s);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == -1);
    }

    SECTION("With extra separators")
    {
        wxString s = d + " -1, k1=v1\tk2=\"v, 2\" ,  Name 3 ";
        EMLineExtinf l(s);
        CHECK(l.hasAttributes());
        CHECK(l.hasName());
        CHECK(l.getName() == "Name 3 ");
        CHECK(l.getPreLen() == " ");
        CHECK(l.getAfterAttr() == " ,  ");
        CHECK(l.toString() == s);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == -1);
    }

    SECTION("Without length")
    {
        wxString s = d + " k1=v1\tk2=\"v, 2\" ,  Name 4 ";
        EMLineExtinf l(s);
        CHECK(l.hasAttributes());
        CHECK(l.hasName());
        CHECK(l.getName() == "Name 4 ");
        CHECK(l.getPreLen() == "");
        CHECK(l.getAfterAttr() == " ,  ");
        CHECK(l.toString() == s);
        CHECK_FALSE(l.hasLen());
    }

    SECTION("Without name")
    {
        wxString s = d + " -1, k1=v1\tk2=\"v, 2\"  ";
        EMLineExtinf l(s);
        CHECK(l.hasAttributes());
        CHECK_FALSE(l.hasName());
        CHECK(l.getPreLen() == " ");
        CHECK(l.getAfterAttr() == "  ");
        CHECK(l.toString() == s);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == -1);
    }
}

TEST_CASE("Editing and generating EXTINF", "[edit][generate]")
{
    EMLineExtinf l = EMLineExtinf::makeEmpty();
    REQUIRE_FALSE(l.hasLen());
    REQUIRE_FALSE(l.hasAttributes());
    REQUIRE_FALSE(l.hasName());
    REQUIRE(l.toString() == "#EXTINF:");

    SECTION("Adding length")
    {
        l.setLen(123);
        REQUIRE(l.hasLen());
        CHECK(l.getLen() == 123);
        CHECK(l.toString() == "#EXTINF:123");
    }

    SECTION("Adding name")
    {
        l.setName("Name");
        REQUIRE(l.hasName());
        CHECK(l.getName() == "Name");
        CHECK(l.getAfterAttr() == ",");
        CHECK(l.toString() == "#EXTINF:,Name");
    }

    SECTION("Adding attributes")
    {
        l.getAttributes().emplace_back(" key=\"val\"");
        REQUIRE(l.hasAttributes());
        CHECK(l.toString() == "#EXTINF: key=\"val\"");
    }

    SECTION("Adding length, attributes and name")
    {
        l.setLen(123);
        l.getAttributes().emplace_back(" key=\"val\"");
        l.setName("Name");
        CHECK(l.toString() == "#EXTINF:123 key=\"val\",Name");
    }
}

TEST_CASE("Normalizing EXTINF", "[normalize]")
{
    EMLineExtinf l("#EXTINF: 123, k1=v1  k2=\"v, 2\" k3 ,\t Name  ");
    l.normalize();
    CHECK(l.toString() == "#EXTINF:123 k1=\"v1\" k2=\"v, 2\" k3,Name");
}

TEST_CASE("Playlist entry", "[parse][generate][normalize]")
{
    std::vector<std::unique_ptr<EMLine>> lines;
    lines.push_back(std::make_unique<EMLineDirective>("#DIR "));
    lines.push_back(std::make_unique<EMLineText>(""));
    lines.push_back(std::make_unique<EMLineExtinf>("#EXTINF: 123, Name"));
    lines.push_back(std::make_unique<EMLineURL>(" folder/file.avi"));

    EMEntry e(std::move(lines));
    REQUIRE(e.hasExtinf());
    CHECK(e.getExtinf().toString() == "#EXTINF: 123, Name");
    REQUIRE(e.hasURL());
    CHECK(e.getURL().toString() == " folder/file.avi");

    e.normalize();
    CHECK(e.getLines()[0]->toString() == "#DIR");
    CHECK(e.getExtinf().toString() == "#EXTINF:123,Name");
    CHECK(e.getURL().toString() == "folder/file.avi");

    auto as=e.toText();
    REQUIRE(as.size() == 4);
    CHECK(as[0] == "#DIR");
    CHECK(as[1] == "");
    CHECK(as[2] == "#EXTINF:123,Name");
    CHECK(as[3] == "folder/file.avi");
}

TEST_CASE("Determining line type", "[parse]")
{
    auto l = EMLine::parse("");
    CHECK(typeid(*l).name() == typeid(EMLineText).name());
    l = EMLine::parse("\t  ");
    CHECK(typeid(*l).name() == typeid(EMLineText).name());
    l = EMLine::parse("#qwerty");
    CHECK(typeid(*l).name() == typeid(EMLineText).name());
    l = EMLine::parse("# qwerty");
    CHECK(typeid(*l).name() == typeid(EMLineText).name());
    l = EMLine::parse("#DIR");
    CHECK(typeid(*l).name() == typeid(EMLineDirective).name());
    l = EMLine::parse("#DIR ");
    CHECK(typeid(*l).name() == typeid(EMLineDirective).name());
    l = EMLine::parse("#DIR: key=val");
    CHECK(typeid(*l).name() == typeid(EMLineDirective).name());
    l = EMLine::parse("#EXTINF:");
    CHECK(typeid(*l).name() == typeid(EMLineExtinf).name());
    l = EMLine::parse("#EXTINF:-1 attr=\"val\",Name");
    CHECK(typeid(*l).name() == typeid(EMLineExtinf).name());
    l = EMLine::parse("#EXTINF: -1, attr=val , Name  ");
    CHECK(typeid(*l).name() == typeid(EMLineExtinf).name());
    l = EMLine::parse("ftp://192.168.0.8/some folder/file.mp3");
    CHECK(typeid(*l).name() == typeid(EMLineURL).name());
    l = EMLine::parse("   C:/some folder/file.mp3 ");
    CHECK(typeid(*l).name() == typeid(EMLineURL).name());
}

TEST_CASE("File data", "[parse][generate][normalize]")
{
    wxArrayString t;
    auto CheckData = [&t](auto& d)
    {
        wxArrayString dt = d.toText();
        REQUIRE(dt.size() == t.size());
        for(std::size_t i=0; i<t.size(); ++i)
        {
            INFO(i);
            CHECK(dt[i] == t[i]);
        }
    };

    SECTION("Empty file")
    {
        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 0);
        CheckData(d);

        d.normalize();
        t.emplace_back("#EXTM3U");
        t.emplace_back("");
        CheckData(d);
    }

    SECTION("Header only")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 0);
        CheckData(d);

        d.normalize();
        t.emplace_back("");
        CHECK(d.getHeader().size() == 1);
        CHECK(d.getHeader()[0]->toString() == t[0]);
        CHECK(d.getEnding().size() == 1);
        CHECK(d.getEnding()[0]->toString() == "");
        CheckData(d);
    }

    SECTION("Header and text")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        t.emplace_back("#qwerty");
        t.emplace_back("");
        t.emplace_back("#aaaaaaaa");
        t.emplace_back("");

        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 0);
        CHECK(d.getEntries().size() == 0);
        CHECK(d.getHeader().size() == 2);
        CHECK(d.getEnding().size() == 3);
        CheckData(d);

        d.normalize();
        CheckData(d);
    }

    SECTION("One entry")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        t.emplace_back("#EXTINF:123,Name");
        t.emplace_back("http://site.com/file.mp3");
        t.emplace_back("");

        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 0);
        CHECK(d.getHeader().size() == 1);
        CHECK(d.getEnding().size() == 1);
        REQUIRE(d.getEntries().size() == 1);
        CHECK(d.getEntries()[0].getLines().size() == 2);
        CheckData(d);

        d.normalize();
        CheckData(d);
    }

    SECTION("Simple list")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        t.emplace_back("#EXTINF:123,Name 1");
        t.emplace_back("http://site.com/music.mp3");
        t.emplace_back("#EXTINF:456,Name 2");
        t.emplace_back("folder/video.avi");
        t.emplace_back("#EXTINF:-1,Name 3");
        t.emplace_back("http://site.com/list.m3u");
        t.emplace_back("");

        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 0);
        CHECK(d.getHeader().size() == 1);
        CHECK(d.getEnding().size() == 1);
        CHECK(d.getEntries().size() == 3);
        for(auto& e : d.getEntries())
        {
            CHECK(e.getLines().size() == 2);
        }
        CheckData(d);

        d.normalize();
        CheckData(d);
    }

    SECTION("Simple list with delimiter line")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        t.emplace_back("");
        t.emplace_back("#EXTINF:123,Name 1");
        t.emplace_back("http://site.com/music.mp3");
        t.emplace_back("");
        t.emplace_back("#EXTINF:456,Name 2");
        t.emplace_back("folder/video.avi");
        t.emplace_back("");
        t.emplace_back("#EXTINF:-1,Name 3");
        t.emplace_back("http://site.com/list.m3u");
        t.emplace_back("");

        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 1);
        CHECK(d.getHeader().size() == 1);
        CHECK(d.getEnding().size() == 0);
        CHECK(d.getEntries().size() == 3);
        for(auto& e : d.getEntries())
        {
            CHECK(e.getLines().size() == 2);
        }
        CheckData(d);

        d.normalize();
        CheckData(d);
    }

    SECTION("Complex list")
    {
        t.emplace_back("#EXTM3U key=\"val\"");
        t.emplace_back("#comment");
        t.emplace_back("");
        t.emplace_back("");
        t.emplace_back("#EXTINF:123,Name 1");
        t.emplace_back("#DIR k=\"v\"");
        t.emplace_back("http://site.com/music.mp3");
        t.emplace_back("# some comments");
        t.emplace_back("");
        t.emplace_back("");
        t.emplace_back("");
        t.emplace_back("#DIR2: key");
        t.emplace_back("#EXTINF:456,Name 2");
        t.emplace_back("folder/video.avi");
        t.emplace_back("");
        t.emplace_back("");
        t.emplace_back("#comments");
        t.emplace_back("");
        t.emplace_back("#EXTINF:-1,Name 3");
        t.emplace_back("http://site.com/list.m3u");
        t.emplace_back("");
        t.emplace_back("# last comment");
        t.emplace_back("");

        EMFileData d(t);
        CHECK(d.getCommonDelimSize() == 2);
        CHECK(d.getHeader().size() == 2);
        CHECK_FALSE(d.getCommonDelimAtEnd());
        CHECK(d.getEnding().size() == 3);
        CHECK(d.getEntries().size() == 3);
        for(auto& e : d.getEntries())
        {
            CHECK(e.getLines().size() == 4);
        }
        CheckData(d);

        d.normalize();
        CheckData(d);
    }
}

