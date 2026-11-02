interface RouteStrategy {
    void findRoute();
}

class ShortestRoute implements RouteStrategy {
    public void findRoute() {
        System.out.println("Shortest Route: Road A");
        System.out.println("Distance: 5 km | Estimated Time: 20 minutes");
    }
}

class FastestRoute implements RouteStrategy {
    public void findRoute() {
        System.out.println("Fastest Route: Road B");
        System.out.println("Distance: 8 km | Estimated Time: 12 minutes");
    }
}

class RoutePlanner {
    RouteStrategy strategy;

    void setStrategy(RouteStrategy strategy) {
        this.strategy = strategy;
    }

    void planRoute() {
        strategy.findRoute();
    }
}


