import java.util.ArrayList;

interface CampusObserver {
    void update(String message);
}

class StudentPortal implements CampusObserver {
    Student student;

    StudentPortal(Student s) {
        student = s;
    }

    public void update(String message) {
        student.notifications.add(message);
    }
}

class NoticePublisher {
    ArrayList<CampusObserver> observers = new ArrayList<>();

    void addObserver(CampusObserver o) {
        observers.add(o);
    }

    void publish(String message) {
        for (CampusObserver o : observers)
            o.update(message);

        System.out.println("Notification published!");
    }
}
