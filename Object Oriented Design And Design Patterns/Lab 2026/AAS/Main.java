import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        SensorFactory factory = new SensorFactory(sc);
        ExternalWeatherAPI api = new ExternalWeatherAPI();
        IrrigationPump pump = new IrrigationPump();
        AlertPublisher publisher = new AlertPublisher();
        FarmDataManager manager = new FarmDataManager();

        publisher.subscribe(new FarmerNotifier());
        publisher.subscribe(new FarmDashboard());

        IrrigationStrategy strategy = new NormalIrrigationStrategy();

        while (true) {
            System.out.println("\n===== SMART IRRIGATION SYSTEM =====");
            System.out.println("1. Add Farm Field");
            System.out.println("2. View Farm Fields");
            System.out.println("3. Select Irrigation Strategy");
            System.out.println("4. Check Irrigation");
            System.out.println("5. Exit");

            System.out.print("Enter choice: ");
            int choice = sc.nextInt();
            sc.nextLine();

            switch (choice) {
                case 1:
                    System.out.print("Enter field name: ");
                    String name = sc.nextLine();
                    manager.addField(new Field(name));
                    break;
                case 2:
                    manager.showFields();
                    break;
                case 3:
                    System.out.println("1. Normal Strategy");
                    System.out.println("2. Water Saving Strategy");
                    System.out.print("Choose strategy: ");

                    int s = sc.nextInt();

                    if (s == 1) {
                        strategy = new NormalIrrigationStrategy();
                        System.out.println("Normal Strategy Selected!");
                    }
                    else if (s == 2) {
                        strategy = new WaterSavingStrategy();
                        System.out.println("Water Saving Strategy Selected!");
                    }
                    else {
                        System.out.println("Invalid choice!");
                    }
                    break;
                case 4:
                    System.out.println("1. Soil Moisture Sensor");
                    System.out.println("2. Temperature Sensor");
                    System.out.print("Choose sensor: ");

                    Sensor sensor = factory.createSensor(sc.nextInt());

                    if (sensor == null) {
                        System.out.println("Invalid sensor choice!");
                        break;
                    }

                    System.out.print("Rain expected? (1=Yes, 0=No): ");
                    int rain = sc.nextInt();

                    if (rain != 0 && rain != 1) {
                        System.out.println("Invalid weather choice!");
                        break;
                    }

                    WeatherService weather =
                        new WeatherAPIAdapter(api, rain == 1);

                    IrrigationController controller =
                        new IrrigationController(strategy, weather, pump, publisher);

                    controller.check(sensor);
                    break;
                case 5:
                    System.out.println("Thank you for using Smart Irrigation System!");
                    sc.close();
                    return;
                default:
                    System.out.println("Invalid choice!");
            }
        }
    }
}
