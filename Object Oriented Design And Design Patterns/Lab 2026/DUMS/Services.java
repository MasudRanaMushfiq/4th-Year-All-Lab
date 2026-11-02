interface UrbanService {
    void provideService();
}

class WasteService implements UrbanService {
    public void provideService() {
        System.out.println("Waste service selected.");
    }
}

class TrafficService implements UrbanService {
    public void provideService() {
        System.out.println("Traffic service selected.");
    }
}

class EmergencyService implements UrbanService {
    public void provideService() {
        System.out.println("Emergency service selected.");
    }
}

class UrbanServiceFactory {
    static UrbanService create(String type) {
        if (type.equalsIgnoreCase("waste"))
            return new WasteService();
        if (type.equalsIgnoreCase("traffic"))
            return new TrafficService();
        if (type.equalsIgnoreCase("emergency"))
            return new EmergencyService();
        return null;
    }
}


