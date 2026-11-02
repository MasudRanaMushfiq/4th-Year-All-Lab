class ExternalTrafficAPI {
    String getTrafficData() {
        return "Road A: Low Traffic, Road B: High Traffic, Road C: Medium Traffic";
    }
}

interface TrafficServiceAPI {
    void showTrafficData();
}

class TrafficAPIAdapter implements TrafficServiceAPI {
    ExternalTrafficAPI api = new ExternalTrafficAPI();

    public void showTrafficData() {
        System.out.println("\nTraffic Information:");
        System.out.println(api.getTrafficData());
    }
}


