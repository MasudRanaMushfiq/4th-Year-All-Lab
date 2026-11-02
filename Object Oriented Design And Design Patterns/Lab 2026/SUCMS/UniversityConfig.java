class UniversityConfig {
    static UniversityConfig obj = new UniversityConfig();
    int maxCredit = 20;

    private UniversityConfig() {}

    static UniversityConfig getInstance() {
        return obj;
    }
}
