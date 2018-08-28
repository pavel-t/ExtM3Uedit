/***************************************************************
 * Name:      tests_file.cpp
 * Purpose:   Unit tests for playlist files access
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-27
 * Copyright: Pavel Tyunin
 * License:
 **************************************************************/

#include "catch.hpp"
#include "../src/em_file.h"
#include <wx/file.h>

TEST_CASE("Files reading and writing", "[file]")
{
    auto CheckFile = [](const wxArrayString& lines)
    {
        wxString fn = "test.txt";
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
