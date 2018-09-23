/***************************************************************
 * Name:      tests_editor.cpp
 * Purpose:   Unit tests for editing interface
 * Author:    Pavel Tyunin (pavel51tunin@gmail.com)
 * Created:   2018-08-27
 * Copyright: Pavel Tyunin
 * License:   GNU General Public License v3.0
 **************************************************************/

#include "catch.hpp"
#include "../src/em_editor.h"
#include <wx/file.h>

TEST_CASE("Playlist editing", "[edit][file][parse][generate]")
{
    wxArrayString d;
    d.emplace_back("#EXTM3U");
    d.emplace_back("#EXTINF:0,Name 1");
    d.emplace_back("file1.mp3");
    d.emplace_back("#EXTINF:123,Name 2");
    d.emplace_back("http://site.com/file2.mkv");
    d.emplace_back("#EXTINF:123 attr=\"qwerty\",Name 3");
    d.emplace_back("ftp://192.168.8.8/folder/file3.avi");
    d.emplace_back("#EXTINF:-1,Name 4");
    d.emplace_back("folder/file4.mp4");
    d.emplace_back();

    auto fd = wxJoin(d, '\n');
    wxString fn = "test.txt";
    {
        wxFile rf;
        REQUIRE(rf.Create(fn, true));
        REQUIRE(rf.Write(fd));
    }

    EMEditor e;
    CHECK(e.getEntryNumber() == 0);
    CHECK_FALSE(e.isSaveable());
    e.openFile(fn);
    REQUIRE(e.getFileName() == fn);
    REQUIRE(e.getEntryNumber() == 4);
    CHECK(e.isSaveable());
    CHECK(e.getName(2) == "Name 3");
    CHECK(e.getURL(2) == "ftp://192.168.8.8/folder/file3.avi");
    CHECK(e.getLength(2) == "123");
    REQUIRE(e.getAttrNumber(2) == 1);
    CHECK(e.getAttrKey(2, 0) == "attr");
    CHECK(e.getAttrVal(2, 0) == "qwerty");

    e.addAttr(2, 1, "newattr");
    REQUIRE(e.getAttrNumber(2) == 2);
    CHECK(e.getAttrKey(2, 1) == "newattr");
    e.setAttrVal(2, 1, "asdf");
    CHECK(e.getAttrVal(2, 1) == "asdf");

    e.removeAttr(2, 0);
    REQUIRE(e.getAttrNumber(2) == 1);
    CHECK(e.getAttrKey(2, 0) == "newattr");
    CHECK(e.getAttrVal(2, 0) == "asdf");
    e.removeAttr(2, 0);
    CHECK(e.getAttrNumber(2) == 0);

    e.addEntry(0, "new file.avi");
    REQUIRE(e.getEntryNumber() == 5);
    CHECK(e.getURL(0) == "new file.avi");

    e.setName(0, "Name 3");
    e.setLength(0, 123);
    e.addAttr(0, 0, "attr");
    e.setAttrVal(0, 0, "qwerty");
    e.setURL(0, "ftp://192.168.8.8/folder/file3.avi");
    e.removeEntry(3);
    REQUIRE(e.getEntryNumber() == 4);
    e.moveEntryDown(0);
    e.moveEntryUp(2);
    e.saveFile();
    e.createFile();
    CHECK(e.getEntryNumber() == 0);
    CHECK_FALSE(e.isSaveable());

    {
        wxFile rf(fn);
        wxString tmp;
        rf.ReadAll(&tmp);
        tmp.Replace("\r\n", '\n');
        tmp.Replace('\r', '\n');
        REQUIRE(tmp == fd);
    }
    wxRemoveFile(fn);
}
