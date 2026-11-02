import java.util.*;

// MAIN CLASS
public class Main {
    static Scanner sc = new Scanner(System.in);

    static HashMap<String, Student> students = new HashMap<>();
    static HashMap<String, Faculty> faculties = new HashMap<>();

    static NoticePublisher publisher = new NoticePublisher();

    // STUDENT REGISTRATION
    static void studentRegistration() {
        System.out.print("Enter ID: ");
        String id = sc.nextLine();

        if (students.containsKey(id)) {
            System.out.println("ID already exists!");
            return;
        }

        System.out.print("Enter Name: ");
        String name = sc.nextLine();

        Student s = (Student) AccountFactory.create("student", id, name);
        students.put(id, s);
        publisher.addObserver(new StudentPortal(s));

        System.out.println("Registration successful!");
    }

    // FACULTY REGISTRATION
    static void facultyRegistration() {
        System.out.print("Enter ID: ");
        String id = sc.nextLine();

        if (faculties.containsKey(id)) {
            System.out.println("ID already exists!");
            return;
        }

        System.out.print("Enter Name: ");
        String name = sc.nextLine();

        Faculty f = (Faculty) AccountFactory.create("faculty", id, name);
        faculties.put(id, f);

        System.out.println("Registration successful!");
    }

    // STUDENT MENU
    static void studentUI(Student s) {
        while (true) {
            System.out.println("\n--- STUDENT MENU ---");
            System.out.println("1. View Courses");
            System.out.println("2. Register Course");
            System.out.println("3. My Courses and Total Credit");
            System.out.println("4. Notifications");
            System.out.println("5. Logout");

            int ch = Integer.parseInt(sc.nextLine());

            if (ch == 1) {
                RegistrationManager.viewCourses();
            }
            else if (ch == 2) {
                System.out.print("Enter Course Code: ");
                RegistrationManager.register(s, sc.nextLine());
            }
            else if (ch == 3) {
                int total = 0;
                for (Course c : s.myCourses) {
                    System.out.println(c.code + " - " + c.name +
                            " | Credit: " + c.credit);
                    total += c.credit;
                }
                System.out.println("Total Credit: " + total);
            }
            else if (ch == 4) {
                if (s.notifications.isEmpty())
                    System.out.println("No notifications!");
                else
                    for (String n : s.notifications)
                        System.out.println(n);
            }
            else if (ch == 5) {
                break;
            }
        }
    }

    // FACULTY MENU
    static void facultyUI(Faculty f) {
        while (true) {
            System.out.println("\n--- FACULTY MENU ---");
            System.out.println("1. View Students");
            System.out.println("2. Publish Result");
            System.out.println("3. Logout");

            int ch = Integer.parseInt(sc.nextLine());

            if (ch == 1) {
                for (Student s : students.values())
                    System.out.println(s.id + " - " + s.name);
            }
            else if (ch == 2) {
                System.out.print("Enter Student ID: ");
                Student s = students.get(sc.nextLine());

                if (s == null) {
                    System.out.println("Student not found!");
                    continue;
                }

                System.out.print("Obtained Marks: ");
                double obtained = Double.parseDouble(sc.nextLine());

                System.out.print("Total Marks: ");
                double total = Double.parseDouble(sc.nextLine());

                if (total <= 0 || obtained < 0 || obtained > total) {
                    System.out.println("Invalid marks!");
                    continue;
                }

                System.out.println("1. Theory Evaluation");
                System.out.println("2. Lab Evaluation");
                int type = Integer.parseInt(sc.nextLine());

                EvaluationService service = new EvaluationService();

                if (type == 1)
                    service.setStrategy(new TheoryEvaluation());
                else if (type == 2)
                    service.setStrategy(new LabEvaluation());
                else {
                    System.out.println("Invalid choice!");
                    continue;
                }

                double result = service.publishResult(
                        new Marks(obtained, total));

                publisher.publish("Your result is: " + result + "%");
                System.out.println("Result: " + result + "%");
            }
            else if (ch == 3) {
                break;
            }
        }
    }

    // ADMIN MENU
    static void adminUI() {
        while (true) {
            System.out.println("\n--- ADMIN MENU ---");
            System.out.println("1. View Courses");
            System.out.println("2. Add Course");
            System.out.println("3. View Registrations");
            System.out.println("4. Library Information");
            System.out.println("5. Publish Notice");
            System.out.println("6. Back");

            int ch = Integer.parseInt(sc.nextLine());

            if (ch == 1) {
                RegistrationManager.viewCourses();
            }
            else if (ch == 2) {
                System.out.print("Course Code: ");
                String code = sc.nextLine();

                System.out.print("Course Name: ");
                String name = sc.nextLine();

                System.out.print("Credit: ");
                int credit = Integer.parseInt(sc.nextLine());

                System.out.print("Capacity: ");
                int capacity = Integer.parseInt(sc.nextLine());

                boolean exists = false;
                for (Course c : RegistrationManager.courses)
                    if (c.code.equalsIgnoreCase(code))
                        exists = true;

                if (exists || credit <= 0 || capacity <= 0) {
                    System.out.println("Invalid or duplicate course!");
                } else {
                    RegistrationManager.courses.add(
                            new Course(code, name, credit, capacity));
                    System.out.println("Course added!");
                }
            }
            else if (ch == 3) {
                for (Course c : RegistrationManager.courses) {
                    System.out.println("\n" + c.code + " - " + c.name);
                    for (String id : c.students)
                        System.out.println("Student ID: " + id);
                }
            }
            else if (ch == 4) {
                LibraryService library = new LibraryAdapter();
                System.out.println(library.getInformation());
            }
            else if (ch == 5) {
                System.out.print("Enter Notice: ");
                publisher.publish("Notice: " + sc.nextLine());
            }
            else if (ch == 6) {
                break;
            }
        }
    }

    // MAIN MENU
    public static void main(String[] args) {
        RegistrationManager.courses.add(
                new Course("CSE101", "Programming", 3, 40));
        RegistrationManager.courses.add(
                new Course("CSE102", "Database", 3, 30));

        while (true) {
            System.out.println("\n===== SUCMS =====");
            System.out.println("1. Student");
            System.out.println("2. Faculty");
            System.out.println("3. Administrator");
            System.out.println("4. Exit");
            System.out.print("Select Role: ");

            int ch = Integer.parseInt(sc.nextLine());

            if (ch == 1) {
                System.out.println("\n1. Registration");
                System.out.println("2. Login");
                int option = Integer.parseInt(sc.nextLine());

                if (option == 1)
                    studentRegistration();
                else if (option == 2) {
                    System.out.print("Enter Student ID: ");
                    Student s = students.get(sc.nextLine());

                    if (s != null)
                        studentUI(s);
                    else
                        System.out.println("Student not found! Register first.");
                }
            }
            else if (ch == 2) {
                System.out.println("\n1. Registration");
                System.out.println("2. Login");
                int option = Integer.parseInt(sc.nextLine());

                if (option == 1)
                    facultyRegistration();
                else if (option == 2) {
                    System.out.print("Enter Faculty ID: ");
                    Faculty f = faculties.get(sc.nextLine());

                    if (f != null)
                        facultyUI(f);
                    else
                        System.out.println("Faculty not found! Register first.");
                }
            }
            else if (ch == 3) {
                adminUI();
            }
            else if (ch == 4) {
                System.out.println("Goodbye!");
                break;
            }
        }
    }
}
