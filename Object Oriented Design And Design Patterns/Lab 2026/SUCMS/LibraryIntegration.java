class ExternalLibrarySystem {
    String getInfo() {
        return "Total Books: 500\nAvailable Books: 350\nBorrowed Books: 150";
    }
}

interface LibraryService {
    String getInformation();
}

class LibraryAdapter implements LibraryService {
    ExternalLibrarySystem library = new ExternalLibrarySystem();

    public String getInformation() {
        return library.getInfo();
    }
}
