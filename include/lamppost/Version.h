#ifndef LAMPPOST_VERSION_H
#define LAMPPOST_VERSION_H


namespace lp {
  struct Version {
    int mMajor;
    int mMinor;
    int mPatch;
    int mRevision;

    Version()  {
      mMajor = 0;
      mMinor = 0;
      mPatch = 0;
      mRevision = 0;
    }

    Version(int major, int minor, int patch, int revision) {
      mMajor = major;
      mMinor = minor;
      mPatch = patch;
      mRevision = revision;
    }
  };
}


#endif //LAMPPOST_VERSION_H
