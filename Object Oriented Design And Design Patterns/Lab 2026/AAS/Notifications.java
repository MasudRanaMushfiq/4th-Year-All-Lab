import java.util.ArrayList;
import java.util.List;

interface AlertObserver {
    void update(String message);
}

class FarmerNotifier implements AlertObserver {
    public void update(String message) {
        System.out.println("Farmer: " + message);
    }
}

class FarmDashboard implements AlertObserver {
    public void update(String message) {
        System.out.println("Dashboard: " + message);
    }
}

class AlertPublisher {
    List<AlertObserver> observers = new ArrayList<>();

    void subscribe(AlertObserver o) {
        observers.add(o);
    }

    void publish(String message) {
        for (AlertObserver o : observers)
            o.update(message);
    }
}
