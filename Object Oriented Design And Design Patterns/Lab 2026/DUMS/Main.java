import java.util.Scanner;

class Citizen {
    void menu(UrbanDatabase db, StatusPublisher publisher) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter your name: ");
        String name = sc.nextLine();
        int choice;

        do {
            System.out.println("\n--- Citizen Menu ---");
            System.out.println("1. Submit Complaint");
            System.out.println("2. View My Complaints");
            System.out.println("3. Exit");
            System.out.print("Choice: ");
            choice = sc.nextInt();
            sc.nextLine();

            if (choice == 1) {
                System.out.print("Complaint ID: ");
                int id = sc.nextInt();
                sc.nextLine();

                if (db.findComplaint(id) != null) {
                    System.out.println("Complaint ID already exists.");
                    continue;
                }

                System.out.print("Service (waste/traffic/emergency): ");
                String type = sc.nextLine();
                UrbanService service = UrbanServiceFactory.create(type);

                if (service == null) {
                    System.out.println("Invalid service.");
                    continue;
                }

                service.provideService();
                System.out.print("Location: ");
                String location = sc.nextLine();
                db.addComplaint(new Complaint(id, name, type, location));
                publisher.notifyAllUsers("New complaint submitted. ID: " + id);
            } else if (choice == 2) {
                db.showCitizen(name);
            }
        } while (choice != 3);
    }
}

class Administrator {
    void menu(UrbanDatabase db, StatusPublisher publisher) {
        Scanner sc = new Scanner(System.in);
        int choice;

        do {
            System.out.println("\n--- Administrator Menu ---");
            System.out.println("1. View All Complaints");
            System.out.println("2. Update Complaint Status");
            System.out.println("3. Plan Route");
            System.out.println("4. View Traffic Data");
            System.out.println("5. Exit");
            System.out.print("Choice: ");
            choice = sc.nextInt();

            if (choice == 1) {
                db.showAll();
            } else if (choice == 2) {
                System.out.print("Enter Complaint ID: ");
                int id = sc.nextInt();
                Complaint complaint = db.findComplaint(id);

                if (complaint != null) {
                    complaint.status = "Resolved";
                    System.out.println("Complaint status updated to Resolved.");
                    publisher.notifyAllUsers("Complaint " + id + " is now Resolved.");
                } else {
                    System.out.println("Complaint not found.");
                }
            } else if (choice == 3) {
                System.out.println("1. Shortest Route");
                System.out.println("2. Fastest Route");
                System.out.print("Choose route: ");
                int option = sc.nextInt();
                RoutePlanner planner = new RoutePlanner();

                if (option == 1)
                    planner.setStrategy(new ShortestRoute());
                else if (option == 2)
                    planner.setStrategy(new FastestRoute());
                else {
                    System.out.println("Invalid choice.");
                    continue;
                }

                planner.planRoute();
            } else if (choice == 4) {
                TrafficServiceAPI traffic = new TrafficAPIAdapter();
                traffic.showTrafficData();
            }
        } while (choice != 5);
    }
}

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        UrbanDatabase db = new UrbanDatabase();
        StatusPublisher publisher = new StatusPublisher();
        publisher.addObserver(new CitizenNotifier());
        publisher.addObserver(new GovernmentDashboard());

        while (true) {
            System.out.println("\n===== DUMS =====");
            System.out.println("1. Citizen");
            System.out.println("2. Administrator");
            System.out.println("3. Exit");
            System.out.print("Select Role: ");
            int choice = sc.nextInt();
            sc.nextLine();

            if (choice == 1)
                new Citizen().menu(db, publisher);
            else if (choice == 2)
                new Administrator().menu(db, publisher);
            else if (choice == 3) {
                System.out.println("Program ended.");
                break;
            } else
                System.out.println("Invalid choice.");
        }
    }
}


