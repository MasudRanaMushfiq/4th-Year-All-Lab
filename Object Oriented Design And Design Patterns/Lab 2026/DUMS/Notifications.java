import java.util.ArrayList;

interface ComplaintObserver {
    void notifyUser(String message);
}

class CitizenNotifier implements ComplaintObserver {
    public void notifyUser(String message) {
        System.out.println("Citizen Notification: " + message);
    }
}

class GovernmentDashboard implements ComplaintObserver {
    public void notifyUser(String message) {
        System.out.println("Government Dashboard: " + message);
    }
}

class StatusPublisher {
    ArrayList<ComplaintObserver> observers = new ArrayList<>();

    void addObserver(ComplaintObserver observer) {
        observers.add(observer);
    }

    void notifyAllUsers(String message) {
        for (ComplaintObserver observer : observers)
            observer.notifyUser(message);
    }
}


