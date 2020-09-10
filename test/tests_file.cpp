/***************************************************************
 * Name:      tests_file.cpp
 * Purpose:   Unit tests for playlist files access
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-27
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "../dep/catch.hpp"
#include "../src/em_file.h"
#include <wx/file.h>

static wxString fn = "test.txt";

TEST_CASE("Files reading and writing", "[file]")
{
    auto CheckFile = [](const wxArrayString& lines)
    {
        wxString fd = wxJoin(lines, '\n');
        wxArrayString lines_read;
        EMFile f;
        auto DoCheck = [&](auto m)
        {
            INFO(m);
            lines_read = f.open(fn);
            CHECK(f.isOpened());
            CHECK(f.getName() == fn);
            REQUIRE(lines_read.size() == lines.size());
            for(std::size_t i=0; i<lines.size(); ++i)
            {
                REQUIRE(lines_read[i] == lines[i]);
            }
        };

        INFO(fd);
        CHECK_FALSE(f.isOpened());

        {
            wxFile rf;
            REQUIRE(rf.Create(fn, true));
            REQUIRE(rf.Write(fd));
        }
        DoCheck("First read");

        f.save(lines_read);
        DoCheck("After save");

        wxRemoveFile(fn);
        f.saveAs(lines_read, fn);
        CHECK(f.isOpened());
        CHECK(f.getName() == fn);
        DoCheck("After save as");

        {
            wxFile rf(fn);
            wxString tmp;
            rf.ReadAll(&tmp);
            tmp.Replace("\r\n", '\n');
            tmp.Replace('\r', '\n');
            REQUIRE(tmp == fd);
        }

        wxRemoveFile(fn);
    };

    wxArrayString d;
    CheckFile(d);
    d.emplace_back("qwerty");
    CheckFile(d);
    d.emplace_back("asdf");
    CheckFile(d);
    d.emplace_back();
    CheckFile(d);
}

static void CompareFile(std::string d)
{
    wxFile f;
    REQUIRE(f.Open(fn));
    size_t s = d.size();
    CHECK(f.Length() == s);
    std::string buf(s, '\0');
    CHECK(f.Read(buf.data(), s) == (int)s);
    CHECK(d == buf);
}

TEST_CASE("Encoding detection", "[file][encoding]")
{
    auto DoCheck = [](std::string fd, Encoding enc, bool bom, auto info)
    {
        INFO(info);
        size_t s = fd.size();
        {
            wxFile rf;
            REQUIRE(rf.Create(fn, true));
            REQUIRE(rf.Write(fd.data(), s) == s);
        }

        {
            EMFile f;
            auto as = f.open(fn);
            REQUIRE(f.isOpened());
            CHECK(f.getEncoding() == enc);
            CHECK(f.getBOM() == bom);
            f.save(as);
        }

        CompareFile(fd);
        wxRemoveFile(fn);
    };

    DoCheck("", Encoding::UTF8, false, "Empty file -> UTF8");
    DoCheck("qwerty!@#$%^&*()\nasdf\n12345", Encoding::UTF8, false, "ASCII only -> UTF8");
    DoCheck("\xF4\xFB\xE2\xE0", Encoding::ANSI, false, "ANSI");
    DoCheck("\xd1\x84\xd1\x8b\xd0\xb2\xd0\xb0", Encoding::UTF8, false, "UTF8");
    DoCheck("\xEF\xBB\xBF\xd1\x84\xd1\x8b\xd0\xb2\xd0\xb0", Encoding::UTF8, true, "UTF8-BOM");
    DoCheck("\xEF\xBB\xBFqwerty!@#$%^&*()_12345", Encoding::UTF8, true, "UTF8-BOM (ASCII only)");
    DoCheck("\xEF\xBB\xBF", Encoding::UTF8, true, "UTF8-BOM (empty file)");
}

TEST_CASE("Encoding selection", "[file][encoding]")
{
    auto DoCheck = [](wxString in, Encoding enc, bool bom, std::string out, auto info)
    {
        INFO(info);
        {
            EMFile f;
            f.setEncoding(enc);
            f.setBOM(bom);
            wxArrayString as;
            as.Add(in);
            f.saveAs(as, fn);
        }

        CompareFile(out);
        wxRemoveFile(fn);
    };

    DoCheck("", Encoding::ANSI, false, "", "ANSI (empty file)");
    DoCheck("qwerty1234!@#$", Encoding::ANSI, false, "qwerty1234!@#$", "ANSI (ASCII only)");
    DoCheck("\xF4\xFB\xE2\xE0", Encoding::ANSI, false, "\xF4\xFB\xE2\xE0", "ANSI");
    DoCheck("", Encoding::UTF8, false, "", "UTF8 (empty file)");
    DoCheck("qwerty1234!@#$", Encoding::UTF8, false, "qwerty1234!@#$", "UTF8 (ASCII only)");
    DoCheck("\xF4\xFB\xE2\xE0", Encoding::UTF8, false, "\xd1\x84\xd1\x8b\xd0\xb2\xd0\xb0", "UTF8");
    DoCheck("", Encoding::UTF8, true, "\xEF\xBB\xBF", "UTF8-BOM (empty file)");
    DoCheck("qwerty1234!@#$", Encoding::UTF8, true, "\xEF\xBB\xBFqwerty1234!@#$", "UTF8-BOM (ASCII only)");
    DoCheck("\xF4\xFB\xE2\xE0", Encoding::UTF8, true, "\xEF\xBB\xBF\xd1\x84\xd1\x8b\xd0\xb2\xd0\xb0", "UTF8-BOM");
}

TEST_CASE("Newline detection", "[file][encoding]")
{
    auto DoCheck = [](std::string fd, NewlineType newline, auto info)
    {
        INFO(info);
        size_t s = fd.size();
        {
            wxFile rf;
            REQUIRE(rf.Create(fn, true));
            REQUIRE(rf.Write(fd.data(), s) == s);
        }

        {
            EMFile f;
            auto as = f.open(fn);
            REQUIRE(f.isOpened());
            CHECK(f.getNewlineType() == newline);
            f.save(as);
        }

        CompareFile(fd);
        wxRemoveFile(fn);
    };

    DoCheck("", NewlineType::CRLF, "Empty file -> CRLF");
    DoCheck("qwerty 12345 !@#$%^&*", NewlineType::CRLF, "No newlines -> CRLF");
    DoCheck("qwerty\r\n", NewlineType::CRLF, "One line, CRLF");
    DoCheck("qwerty\n", NewlineType::LF, "One line, LF");
    DoCheck("qwerty\r", NewlineType::CR, "One line, CR");

    auto CheckNewlineType = [=](auto& lines, auto eol, NewlineType newline, std::string info)
    {
        std::string fd = lines[0];
        for(size_t i=1; i<lines.size(); ++i)
            fd += (eol + lines[i]);
        DoCheck(fd, newline, info+", no empty last line");
        DoCheck(fd+eol, newline, info+", empty last line");
    };
    auto CheckNewlineTypes = [=](auto& lines, std::string info)
    {
        CheckNewlineType(lines, "\r\n", NewlineType::CRLF, info+", CRLF");
        CheckNewlineType(lines, '\n', NewlineType::LF, info+", LF");
        CheckNewlineType(lines, '\r', NewlineType::CR, info+", CR");
    };
    std::vector<std::string> lines {"qwerty", "asdf"};
    CheckNewlineTypes(lines, "2 lines");
    lines.emplace_back("zxcvbnm");
    CheckNewlineTypes(lines, "3 lines");
    lines.resize(10, "1234567890");
    CheckNewlineTypes(lines, "10 lines");
    lines.resize(100, "7894561230");
    CheckNewlineTypes(lines, "100 lines");
    lines.resize(1000, "qazxswedcvfrtgb");
    CheckNewlineTypes(lines, "1000 lines");
}
