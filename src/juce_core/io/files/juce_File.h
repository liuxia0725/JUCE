/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#ifndef __JUCE_FILE_JUCEHEADER__
#define __JUCE_FILE_JUCEHEADER__

#include "../../containers/juce_OwnedArray.h"
#include "../../basics/juce_Time.h"
#include "../../text/juce_StringArray.h"
#include "../../containers/juce_MemoryBlock.h"
class FileInputStream;
class FileOutputStream;


//==============================================================================
/**
    Represents a local file or directory.

    This class encapsulates the absolute pathname of a file or directory, and
    has methods for finding out about the file and changing its properties.

    To read or write to the file, there are methods for returning an input or
    output stream.

    @see FileInputStream, FileOutputStream
*/
class JUCE_API  File
{
public:
    //==============================================================================
    /** Creates an (invalid) file object.

        The file is initially set to an empty path, so getFullPath() will return
        an empty string, and comparing the file to File::nonexistent will return
        true.

        You can use its operator= method to point it at a proper file.
    */
    File() throw()   {}

    /** Creates a file from an absolute path.

        If the path supplied is a relative path, it is taken to be relative
        to the current working directory (see File::getCurrentWorkingDirectory()),
        but this isn't a recommended way of creating a file, because you
        never know what the CWD is going to be.

        On the Mac/Linux, the path can include "~" notation for referring to
        user home directories.
    */
    File (const String& path) throw();

    /** Creates a copy of another file object. */
    File (const File& other) throw();

    /** Destructor. */
    ~File() throw()  {}

    /** Sets the file based on an absolute pathname.

        If the path supplied is a relative path, it is taken to be relative
        to the current working directory (see File::getCurrentWorkingDirectory()),
        but this isn't a recommended way of creating a file, because you
        never know what the CWD is going to be.

        On the Mac/Linux, the path can include "~" notation for referring to
        user home directories.
    */
    const File& operator= (const String& newFilePath) throw();

    /** Copies from another file object. */
    const File& operator= (const File& otherFile) throw();

    //==============================================================================
    /** This static constant is used for referring to an 'invalid' file. */
    static const File nonexistent;

    //==============================================================================
    /** Checks whether the file actually exists.

        @returns    true if the file exists, either as a file or a directory.
        @see existsAsFile, isDirectory
    */
    bool exists() const throw();

    /** Checks whether the file exists and is a file rather than a directory.

        @returns    true only if this is a real file, false if it's a directory
                    or doesn't exist
        @see exists, isDirectory
    */
    bool existsAsFile() const throw();

    /** Checks whether the file is a directory that exists.

        @returns    true only if the file is a directory which actually exists, so
                    false if it's a file or doesn't exist at all
        @see exists, existsAsFile
    */
    bool isDirectory() const throw();

    /** Returns the size of the file in bytes.

        @returns    the number of bytes in the file, or 0 if it doesn't exist.
    */
    int64 getSize() const throw();

    /** Utility function to convert a file size in bytes to a neat string description.

        So for example 100 would return "100 bytes", 2000 would return "2 KB",
        2000000 would produce "2 MB", etc.
    */
    static const String descriptionOfSizeInBytes (const int64 bytes);

    //==============================================================================
    /** Returns the complete, absolute path of this file.

        This includes the filename and all its parent folders. On Windows it'll
        also include the drive letter prefix; on Mac or Linux it'll be a complete
        path starting from the root folder.

        If you just want the file's name, you should use getFileName() or
        getFileNameWithoutExtension().

        @see getFileName, getRelativePathFrom
    */
    const String& getFullPathName() const throw()    { return fullPath; }

    /** Returns the last section of the pathname.

        Returns just the final part of the path - e.g. if the whole path
        is "/moose/fish/foo.txt" this will return "foo.txt".

        For a directory, it returns the final part of the path - e.g. for the
        directory "/moose/fish" it'll return "fish".

        If the filename begins with a dot, it'll return the whole filename, e.g. for
        "/moose/.fish", it'll return ".fish"

        @see getFullPathName, getFileNameWithoutExtension
    */
    const String getFileName() const throw();

    /** Creates a relative path that refers to a file relatively to a given directory.

        e.g. File ("/moose/foo.txt").getRelativePathFrom ("/moose/fish/haddock")
             would return "../../foo.txt".

        If it's not possible to navigate from one file to the other, an absolute
        path is returned. If the paths are invalid, an empty string may also be
        returned.

        @param directoryToBeRelativeTo  the directory which the resultant string will
                                        be relative to. If this is actually a file rather than
                                        a directory, its parent directory will be used instead.
                                        If it doesn't exist, it's assumed to be a directory.
        @see getChildFile, isAbsolutePath
    */
    const String getRelativePathFrom (const File& directoryToBeRelativeTo) const throw();

    //==============================================================================
    /** Returns the file's extension.

        Returns the file extension of this file, also including the dot.

        e.g. "/moose/fish/foo.txt" would return ".txt"

        @see hasFileExtension, withFileExtension, getFileNameWithoutExtension
    */
    const String getFileExtension() const throw();

    /** Checks whether the file has a given extension.

        @param extensionToTest  the extension to look for - it doesn't matter whether or
                                not this string has a dot at the start, so ".wav" and "wav"
                                will have the same effect. The comparison used is
                                case-insensitve.

        @see getFileExtension, withFileExtension, getFileNameWithoutExtension
    */
    bool hasFileExtension (const String& extensionToTest) const throw();

    /** Returns a version of this file with a different file extension.

        e.g. File ("/moose/fish/foo.txt").withFileExtension ("html") returns "/moose/fish/foo.html"

        @param newExtension     the new extension, either with or without a dot at the start (this
                                doesn't make any difference). To get remove a file's extension altogether,
                                pass an empty string into this function.

        @see getFileName, getFileExtension, hasFileExtension, getFileNameWithoutExtension
    */
    const File withFileExtension (const String& newExtension) const throw();

    /** Returns the last part of the filename, without its file extension.

        e.g. for "/moose/fish/foo.txt" this will return "foo".

        @see getFileName, getFileExtension, hasFileExtension, withFileExtension
    */
    const String getFileNameWithoutExtension() const throw();

    //==============================================================================
    /** Returns a 32-bit hash-code that identifies this file.

        This is based on the filename. Obviously it's possible, although unlikely, that
        two files will have the same hash-code.
    */
    int hashCode() const throw();

    /** Returns a 64-bit hash-code that identifies this file.

        This is based on the filename. Obviously it's possible, although unlikely, that
        two files will have the same hash-code.
    */
    int64 hashCode64() const throw();

    //==============================================================================
    /** Returns a file based on a relative path.

        This will find a child file or directory of the current object.

        e.g.
            File ("/moose/fish").getChildFile ("foo.txt") will produce "/moose/fish/foo.txt".
            File ("/moose/fish").getChildFile ("../foo.txt") will produce "/moose/foo.txt".

        If the string is actually an absolute path, it will be treated as such, e.g.
            File ("/moose/fish").getChildFile ("/foo.txt") will produce "/foo.txt"

        @see getSiblingFile, getParentDirectory, getRelativePathFrom, isAChildOf
    */
    const File getChildFile (String relativePath) const throw();

    /** Returns a file which is in the same directory as this one.

        This is equivalent to getParentDirectory().getChildFile (name).

        @see getChildFile, getParentDirectory
    */
    const File getSiblingFile (const String& siblingFileName) const throw();

    //==============================================================================
    /** Returns the directory that contains this file or directory.

        e.g. for "/moose/fish/foo.txt" this will return "/moose/fish".
    */
    const File getParentDirectory() const throw();

    /** Checks whether a file is somewhere inside a directory.

        Returns true if this file is somewhere inside a subdirectory of the directory
        that is passed in. Neither file actually has to exist, because the function
        just checks the paths for similarities.

        e.g. File ("/moose/fish/foo.txt").isAChildOf ("/moose") is true.
             File ("/moose/fish/foo.txt").isAChildOf ("/moose/fish") is also true.
    */
    bool isAChildOf (const File& potentialParentDirectory) const throw();

    //==============================================================================
    /** Chooses a filename relative to this one that doesn't already exist.

        If this file is a directory, this will return a child file of this
        directory that doesn't exist, by adding numbers to a prefix and suffix until
        it finds one that isn't already there.

        If the prefix + the suffix doesn't exist, it won't bother adding a number.

        e.g. File ("/moose/fish").getNonexistentChildFile ("foo", ".txt", true) might
             return "/moose/fish/foo(2).txt" if there's already a file called "foo.txt".

        @param prefix                   the string to use for the filename before the number
        @param suffix                   the string to add to the filename after the number
        @param putNumbersInBrackets     if true, this will create filenames in the
                                        format "prefix(number)suffix", if false, it will leave the
                                        brackets out.
    */
    const File getNonexistentChildFile (const String& prefix,
                                        const String& suffix,
                                        bool putNumbersInBrackets = true) const throw();

    /** Chooses a filename for a sibling file to this one that doesn't already exist.

        If this file doesn't exist, this will just return itself, otherwise it
        will return an appropriate sibling that doesn't exist, e.g. if a file
        "/moose/fish/foo.txt" exists, this might return "/moose/fish/foo(2).txt".

        @param putNumbersInBrackets     whether to add brackets around the numbers that
                                        get appended to the new filename.
    */
    const File getNonexistentSibling (const bool putNumbersInBrackets = true) const throw();

    //==============================================================================
    /** Compares the pathnames for two files. */
    bool operator== (const File& otherFile) const throw();
    /** Compares the pathnames for two files. */
    bool operator!= (const File& otherFile) const throw();

    //==============================================================================
    /** Checks whether a file can be created or written to.

        @returns    true if it's possible to create and write to this file. If the file
                    doesn't already exist, this will check its parent directory to
                    see if writing is allowed.
        @see setReadOnly
    */
    bool hasWriteAccess() const throw();

    /** Changes the write-permission of a file or directory.

        @param shouldBeReadOnly     whether to add or remove write-permission
        @param applyRecursively     if the file is a directory and this is true, it will
                                    recurse through all the subfolders changing the permissions
                                    of all files
        @returns    true if it manages to change the file's permissions.
        @see hasWriteAccess
    */
    bool setReadOnly (const bool shouldBeReadOnly,
                      const bool applyRecursively = false) const throw();

    /** Returns true if this file is a hidden or system file.

        The criteria for deciding whether a file is hidden are platform-dependent.
    */
    bool isHidden() const throw();

    /** If this file is a link, this returns the file that it points to.

        If this file isn't actually link, it'll just return itself.
    */
    const File getLinkedTarget() const throw();

    //==============================================================================
    /** Returns the last modification time of this file.

        @returns    the time, or an invalid time if the file doesn't exist.
        @see setLastModificationTime, getLastAccessTime, getCreationTime
    */
    const Time getLastModificationTime() const throw();

    /** Returns the last time this file was accessed.

        @returns    the time, or an invalid time if the file doesn't exist.
        @see setLastAccessTime, getLastModificationTime, getCreationTime
    */
    const Time getLastAccessTime() const throw();

    /** Returns the time that this file was created.

        @returns    the time, or an invalid time if the file doesn't exist.
        @see getLastModificationTime, getLastAccessTime
    */
    const Time getCreationTime() const throw();

    /** Changes the modification time for this file.

        @param newTime  the time to apply to the file
        @returns true if it manages to change the file's time.
        @see getLastModificationTime, setLastAccessTime, setCreationTime
    */
    bool setLastModificationTime (const Time& newTime) const throw();

    /** Changes the last-access time for this file.

        @param newTime  the time to apply to the file
        @returns true if it manages to change the file's time.
        @see getLastAccessTime, setLastModificationTime, setCreationTime
    */
    bool setLastAccessTime (const Time& newTime) const throw();

    /** Changes the creation date for this file.

        @param newTime  the time to apply to the file
        @returns true if it manages to change the file's time.
        @see getCreationTime, setLastModificationTime, setLastAccessTime
    */
    bool setCreationTime (const Time& newTime) const throw();

    /** If possible, this will try to create a version string for the given file.

        The OS may be able to look at the file and give a version for it - e.g. with
        executables, bundles, dlls, etc. If no version is available, this will
        return an empty string.
    */
    const String getVersion() const throw();

    //==============================================================================
    /** Creates an empty file if it doesn't already exist.

        If the file that this object refers to doesn't exist, this will create a file
        of zero size.

        If it already exists or is a directory, this method will do nothing.

        @returns    true if the file has been created (or if it already existed).
        @see createDirectory
    */
    bool create() const throw();

    /** Creates a new directory for this filename.

        This will try to create the file as a directory, and fill also create
        any parent directories it needs in order to complete the operation.

        @returns    true if the directory has been created successfully, (or if it
                    already existed beforehand).
        @see create
    */
    bool createDirectory() const throw();

    /** Deletes a file.

        If this file is actually a directory, it may not be deleted correctly if it
        contains files. See deleteRecursively() as a better way of deleting directories.

        @returns    true if the file has been successfully deleted (or if it didn't exist to
                    begin with).
        @see deleteRecursively
    */
    bool deleteFile() const throw();

    /** Deletes a file or directory and all its subdirectories.

        If this file is a directory, this will try to delete it and all its subfolders. If
        it's just a file, it will just try to delete the file.

        @returns    true if the file and all its subfolders have been successfully deleted
                    (or if it didn't exist to begin with).
        @see deleteFile
    */
    bool deleteRecursively() const throw();

    /** Moves this file or folder to the trash.

        @returns true if the operation succeeded. It could fail if the trash is full, or
                 if the file is write-protected, so you should check the return value
                 and act appropriately.
    */
    bool moveToTrash() const throw();

    /** Moves or renames a file.

        Tries to move a file to a different location.
        If the target file already exists, this will attempt to delete it first, and
        will fail if this can't be done.

        Note that the destination file isn't the directory to put it in, it's the actual
        filename that you want the new file to have.

        @returns    true if the operation succeeds
    */
    bool moveFileTo (const File& targetLocation) const throw();

    /** Copies a file.

        Tries to copy a file to a different location.
        If the target file already exists, this will attempt to delete it first, and
        will fail if this can't be done.

        @returns    true if the operation succeeds
    */
    bool copyFileTo (const File& targetLocation) const throw();

    /** Copies a directory.

        Tries to copy an entire directory, recursively.

        If this file isn't a directory or if any target files can't be created, this
        will return false.

        @param newDirectory    the directory that this one should be copied to. Note that this
                               is the name of the actual directory to create, not the directory
                               into which the new one should be placed, so there must be enough
                               write privileges to create it if it doesn't exist. Any files inside
                               it will be overwritten by similarly named ones that are copied.
    */
    bool copyDirectoryTo (const File& newDirectory) const throw();

    //==============================================================================
    /** Used in file searching, to specify whether to return files, directories, or both.
    */
    enum TypesOfFileToFind
    {
        findDirectories             = 1,    /**< Use this flag to indicate that you want to find directories. */
        findFiles                   = 2,    /**< Use this flag to indicate that you want to find files. */
        findFilesAndDirectories     = 3,    /**< Use this flag to indicate that you want to find both files and directories. */
        ignoreHiddenFiles           = 4     /**< Add this flag to avoid returning any hidden files in the results. */
    };

    /** Searches inside a directory for files matching a wildcard pattern.

        Assuming that this file is a directory, this method will search it
        for either files or subdirectories whose names match a filename pattern.

        @param results                  an array to which File objects will be added for the
                                        files that the search comes up with
        @param whatToLookFor            a value from the TypesOfFileToFind enum, specifying whether to
                                        return files, directories, or both. If the ignoreHiddenFiles flag
                                        is also added to this value, hidden files won't be returned
        @param searchRecursively        if true, all subdirectories will be recursed into to do
                                        an exhaustive search
        @param wildCardPattern          the filename pattern to search for, e.g. "*.txt"
        @returns                        the number of results that have been found

        @see getNumberOfChildFiles, DirectoryIterator
    */
    int findChildFiles (OwnedArray<File>& results,
                        const int whatToLookFor,
                        const bool searchRecursively,
                        const String& wildCardPattern = JUCE_T("*")) const throw();

    /** Searches inside a directory and counts how many files match a wildcard pattern.

        Assuming that this file is a directory, this method will search it
        for either files or subdirectories whose names match a filename pattern,
        and will return the number of matches found.

        This isn't a recursive call, and will only search this directory, not
        its children.

        @param whatToLookFor    a value from the TypesOfFileToFind enum, specifying whether to
                                count files, directories, or both. If the ignoreHiddenFiles flag
                                is also added to this value, hidden files won't be counted
        @param wildCardPattern  the filename pattern to search for, e.g. "*.txt"
        @returns                the number of matches found
        @see findChildFiles, DirectoryIterator
    */
    int getNumberOfChildFiles (const int whatToLookFor,
                               const String& wildCardPattern = JUCE_T("*")) const throw();

    /** Returns true if this file is a directory that contains one or more subdirectories.
        @see isDirectory, findChildFiles
    */
    bool containsSubDirectories() const throw();

    //==============================================================================
    /** Creates a stream to read from this file.

        @returns    a stream that will read from this file (initially positioned at the
                    start of the file), or 0 if the file can't be opened for some reason
        @see createOutputStream, loadFileAsData
    */
    FileInputStream* createInputStream() const throw();

    /** Creates a stream to write to this file.

        If the file exists, the stream that is returned will be positioned ready for
        writing at the end of the file, so you might want to use deleteFile() first
        to write to an empty file.

        @returns    a stream that will write to this file (initially positioned at the
                    end of the file), or 0 if the file can't be opened for some reason
        @see createInputStream, printf, appendData, appendText
    */
    FileOutputStream* createOutputStream (const int bufferSize = 0x8000) const throw();

    //==============================================================================
    /** Loads a file's contents into memory as a block of binary data.

        Of course, trying to load a very large file into memory will blow up, so
        it's better to check first.

        @param result   the data block to which the file's contents should be appended - note
                        that if the memory block might already contain some data, you
                        might want to clear it first
        @returns        true if the file could all be read into memory
    */
    bool loadFileAsData (MemoryBlock& result) const throw();

    /** Reads a file into memory as a string.

        Attempts to load the entire file as a zero-terminated string.

        This makes use of InputStream::readEntireStreamAsString, which should
        automatically cope with unicode/acsii file formats.
    */
    const String loadFileAsString() const throw();

    //==============================================================================
    /** Writes text to the end of the file.

        This will try to do a printf to the file.

        @returns  false if it can't write to the file for some reason
    */
    bool printf (const tchar* format, ...) const throw();

    /** Appends a block of binary data to the end of the file.

        This will try to write the given buffer to the end of the file.

        @returns false if it can't write to the file for some reason
    */
    bool appendData (const void* const dataToAppend,
                     const int numberOfBytes) const throw();

    /** Replaces this file's contents with a given block of data.

        This will delete the file and replace it with the given data.

        A nice feature of this method is that it's safe - instead of deleting
        the file first and then re-writing it, it creates a new temporary file,
        writes the data to that, and then moves the new file to replace the existing
        file. This means that if the power gets pulled out or something crashes,
        you're a lot less likely to end up with an empty file..

        Returns true if the operation succeeds, or false if it fails.

        @see appendText
    */
    bool replaceWithData (const void* const dataToWrite,
                          const int numberOfBytes) const throw();

    /** Appends a string to the end of the file.

        This will try to append a text string to the file, as either 16-bit unicode
        or 8-bit characters in the default system encoding.

        It can also write the 'ff fe' unicode header bytes before the text to indicate
        the endianness of the file.

        Any single \\n characters in the string are replaced with \\r\\n before it is written.

        @see replaceWithText
    */
    bool appendText (const String& textToAppend,
                     const bool asUnicode = false,
                     const bool writeUnicodeHeaderBytes = false) const throw();

    /** Replaces this file's contents with a given text string.

        This will delete the file and replace it with the given text.

        A nice feature of this method is that it's safe - instead of deleting
        the file first and then re-writing it, it creates a new temporary file,
        writes the text to that, and then moves the new file to replace the existing
        file. This means that if the power gets pulled out or something crashes,
        you're a lot less likely to end up with an empty file..

        For an explanation of the parameters here, see the appendText() method.

        Returns true if the operation succeeds, or false if it fails.

        @see appendText
    */
    bool replaceWithText (const String& textToWrite,
                          const bool asUnicode = false,
                          const bool writeUnicodeHeaderBytes = false) const throw();

    //==============================================================================
    /** Creates a set of files to represent each file root.

        e.g. on Windows this will create files for "c:\", "d:\" etc according
        to which ones are available. On the Mac/Linux, this will probably
        just add a single entry for "/".
    */
    static void findFileSystemRoots (OwnedArray<File>& results) throw();

    /** Finds the name of the drive on which this file lives.

        @returns the volume label of the drive, or an empty string if this isn't possible
    */
    const String getVolumeLabel() const throw();

    /** Returns the serial number of the volume on which this file lives.

        @returns the serial number, or zero if there's a problem doing this
    */
    int getVolumeSerialNumber() const throw();

    /** Returns the number of bytes free on the drive that this file lives on.

        @returns the number of bytes free, or 0 if there's a problem finding this out
        @see getVolumeTotalSize
    */
    int64 getBytesFreeOnVolume() const throw();

    /** Returns the total size of the drive that contains this file.

        @returns the total number of bytes that the volume can hold
        @see getBytesFreeOnVolume
    */
    int64 getVolumeTotalSize() const throw();

    /** Returns true if this file is on a CD or DVD drive. */
    bool isOnCDRomDrive() const throw();

    /** Returns true if this file is on a hard disk.

        This will fail if it's a network drive, but will still be true for
        removable hard-disks.
    */
    bool isOnHardDisk() const throw();

    /** Returns true if this file is on a removable disk drive.

        This might be a usb-drive, a CD-rom, or maybe a network drive.
    */
    bool isOnRemovableDrive() const throw();

    //==============================================================================
    /** Launches the file as a process.

        - if the file is executable, this will run it.

        - if it's a document of some kind, it will launch the document with its
        default viewer application.

        - if it's a folder, it will be opened in Explorer, Finder, or equivalent.
    */
    bool startAsProcess (const String& parameters = String::empty) const throw();

    //==============================================================================
    /** A set of types of location that can be passed to the getSpecialLocation() method.
    */
    enum SpecialLocationType
    {
        /** The user's home folder. This is the same as using File ("~"). */
        userHomeDirectory,

        /** The user's default documents folder. On Windows, this might be the user's
            "My Documents" folder. On the Mac it'll be their "Documents" folder. Linux
            doesn't tend to have one of these, so it might just return their home folder.
        */
        userDocumentsDirectory,

        /** The folder that contains the user's desktop objects. */
        userDesktopDirectory,

        /** The folder in which applications store their persistent user-specific settings.
            On Windows, this might be "\Documents and Settings\username\Application Data".
            On the Mac, it might be "~/Library". If you're going to store your settings in here,
            always create your own sub-folder to put them in, to avoid making a mess.
        */
        userApplicationDataDirectory,

        /** An equivalent of the userApplicationDataDirectory folder that is shared by all users
            of the computer, rather than just the current user.

            On the Mac it'll be "/Library", on Windows, it could be something like
            "\Documents and Settings\All Users\Application Data".

            Depending on the setup, this folder may be read-only.
        */
        commonApplicationDataDirectory,

        /** The folder that should be used for temporary files.

            Always delete them when you're finished, to keep the user's computer tidy!
        */
        tempDirectory,

        /** Returns this application's executable file.

            If running as a plug-in or DLL, this will (where possible) be the DLL rather than the
            host app.

            On the mac this will return the unix binary, not the package folder - see
            currentApplicationFile for that.
        */
        currentExecutableFile,

        /** Returns this application's location.

            If running as a plug-in or DLL, this will (where possible) be the DLL rather than the
            host app.

            On the mac this will return the package folder (if it's in one), not the unix binary
            that's inside it - compare with currentExecutableFile.
        */
        currentApplicationFile,

        /** The directory in which applications normally get installed.

            So on windows, this would be something like "c:\program files", on the
            Mac "/Applications", or "/usr" on linux.
        */
        globalApplicationsDirectory,

        /** The most likely place where a user might store their music files.
        */
        userMusicDirectory,

        /** The most likely place where a user might store their movie files.
        */
        userMoviesDirectory,
    };

    /** Finds the location of a special type of file or directory, such as a home folder or
        documents folder.

        @see SpecialLocationType
    */
    static const File JUCE_CALLTYPE getSpecialLocation (const SpecialLocationType type);

    //==============================================================================
    /** Returns a temporary file in the system's temp directory.

        This will try to return the name of a non-existent temp file.

        To get the temp folder, you can use getSpecialLocation (File::tempDirectory).
    */
    static const File createTempFile (const String& fileNameEnding) throw();


    //==============================================================================
    /** Returns the current working directory.

        @see setAsCurrentWorkingDirectory
    */
    static const File getCurrentWorkingDirectory() throw();

    /** Sets the current working directory to be this file.

        For this to work the file must point to a valid directory.

        @returns true if the current directory has been changed.
        @see getCurrentWorkingDirectory
    */
    bool setAsCurrentWorkingDirectory() const throw();

    //==============================================================================
    /** The system-specific file separator character.

        On Windows, this will be '\', on Mac/Linux, it'll be '/'
    */
    static const tchar separator;

    /** The system-specific file separator character, as a string.

        On Windows, this will be '\', on Mac/Linux, it'll be '/'
    */
    static const tchar* separatorString;

    //==============================================================================
    /** Removes illegal characters from a filename.

        This will return a copy of the given string after removing characters
        that are not allowed in a legal filename, and possibly shortening the
        string if it's too long.

        Because this will remove slashes, don't use it on an absolute pathname.

        @see createLegalPathName
    */
    static const String createLegalFileName (const String& fileNameToFix) throw();

    /** Removes illegal characters from a pathname.

        Similar to createLegalFileName(), but this won't remove slashes, so can
        be used on a complete pathname.

        @see createLegalFileName
    */
    static const String createLegalPathName (const String& pathNameToFix) throw();

    /** Indicates whether filenames are case-sensitive on the current operating system.
    */
    static bool areFileNamesCaseSensitive();

    /** Returns true if the string seems to be a fully-specified absolute path.
    */
    static bool isAbsolutePath (const String& path) throw();

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //==============================================================================
    String fullPath;

    // internal way of contructing a file without checking the path
    friend class DirectoryIterator;
    File (const String&, int) throw();
    const String getPathUpToLastSlash() const throw();
};

#endif   // __JUCE_FILE_JUCEHEADER__
