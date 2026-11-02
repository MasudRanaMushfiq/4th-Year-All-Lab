import java.util.ArrayList;

class Complaint {
    int id;
    String citizen, type, location, status;

    Complaint(int id, String citizen, String type, String location) {
        this.id = id;
        this.citizen = citizen;
        this.type = type;
        this.location = location;
        status = "Pending";
    }

    void display() {
        System.out.println("ID: " + id + " | Citizen: " + citizen
                + " | Type: " + type + " | Location: " + location
                + " | Status: " + status);
    }
}

class UrbanDatabase {
    ArrayList<Complaint> complaints = new ArrayList<>();

    void addComplaint(Complaint complaint) {
        complaints.add(complaint);
    }

    Complaint findComplaint(int id) {
        for (Complaint complaint : complaints) {
            if (complaint.id == id)
                return complaint;
        }
        return null;
    }

    void showAll() {
        for (Complaint complaint : complaints)
            complaint.display();
    }

    void showCitizen(String name) {
        for (Complaint complaint : complaints) {
            if (complaint.citizen.equalsIgnoreCase(name))
                complaint.display();
        }
    }
}


