Feature: DevTools Manager - Inventory and Stock Management System
  As an IT company administrator
  I want to manage inventory, stock, and internal tools
  So that I can optimize control of resources used by technical teams

  Background:
    Given the DevTools Manager system is running
    And I am logged in as an administrator

  # ============================================================================
  # INVENTORY MANAGEMENT
  # ============================================================================

  Scenario: Add a new item to inventory
    Given I am on the inventory management page
    When I click on "Add New Item"
    And I fill in the item details:
      | Field        | Value                    |
      | Name         | Dell XPS 15 Laptop       |
      | Category     | Hardware                 |
      | Quantity     | 5                        |
      | Unit Price   | 1500.00                  |
      | Supplier     | Dell Technologies        |
      | Description  | High-performance laptop  |
    And I click "Save"
    Then the item should be added to the inventory
    And I should see a success message "Item added successfully"
    And the item should appear in the inventory list

  Scenario: Register incoming items to inventory
    Given the following item exists in inventory:
      | Name            | Current Stock |
      | USB-C Cable     | 10            |
    When I navigate to "Incoming Items"
    And I select item "USB-C Cable"
    And I enter quantity "25"
    And I enter supplier reference "PO-2025-11-001"
    And I add notes "Bulk purchase for Q4"
    And I click "Register Incoming"
    Then the stock quantity should be updated to "35"
    And the incoming transaction should be recorded in history
    And I should see a confirmation "25 units added to stock"

  Scenario: Register outgoing items from inventory
    Given the following item exists in inventory:
      | Name               | Current Stock |
      | Wireless Mouse     | 50            |
    When I navigate to "Outgoing Items"
    And I select item "Wireless Mouse"
    And I enter quantity "5"
    And I select recipient "Development Team A"
    And I enter purpose "New developer workstations"
    And I click "Register Outgoing"
    Then the stock quantity should be decreased to "45"
    And the outgoing transaction should be recorded in history
    And I should see a confirmation "5 units removed from stock"

  Scenario: Prevent outgoing items when stock is insufficient
    Given the following item exists in inventory:
      | Name            | Current Stock |
      | HDMI Adapter    | 3             |
    When I navigate to "Outgoing Items"
    And I select item "HDMI Adapter"
    And I enter quantity "5"
    And I click "Register Outgoing"
    Then I should see an error message "Insufficient stock. Available: 3 units"
    And the stock quantity should remain "3"
    And no transaction should be recorded

  Scenario: Search for items in inventory
    Given the inventory contains the following items:
      | Name                | Category  |
      | MacBook Pro M3      | Hardware  |
      | Microsoft Office    | Software  |
      | Network Switch      | Hardware  |
    When I enter "MacBook" in the search field
    Then I should see 1 result
    And the result should contain "MacBook Pro M3"

  Scenario: Filter inventory by category
    Given the inventory contains items in multiple categories
    When I select filter "Category"
    And I choose "Hardware"
    Then I should see only items in the "Hardware" category
    And software items should not be displayed

  # ============================================================================
  # STOCK CONTROL
  # ============================================================================

  Scenario: Set minimum stock level alert
    Given the item "RAM DDR4 16GB" exists in inventory
    When I navigate to stock settings for "RAM DDR4 16GB"
    And I set minimum stock level to "10"
    And I click "Save Settings"
    Then the minimum stock level should be saved
    And I should see a confirmation "Stock alert configured"

  Scenario: Receive low stock alert
    Given the item "SSD 1TB" has the following settings:
      | Current Stock | Minimum Stock Level |
      | 8             | 10                  |
    When I view the dashboard
    Then I should see a low stock alert for "SSD 1TB"
    And the alert should indicate "Current: 8, Minimum: 10"

  Scenario: View current stock levels
    Given multiple items exist in inventory
    When I navigate to "Stock Control"
    Then I should see a list of all items with their current stock levels
    And each item should display:
      | Field                |
      | Item Name            |
      | Current Quantity     |
      | Minimum Stock Level  |
      | Stock Status         |

  Scenario: Bulk update stock quantities
    Given I am on the stock control page
    When I select multiple items:
      | Item Name      |
      | USB-A Cable    |
      | USB-C Cable    |
      | HDMI Cable     |
    And I click "Bulk Update"
    And I enter adjustment type "Add"
    And I enter quantity "20"
    And I confirm the bulk update
    Then each selected item should have 20 units added
    And all updates should be recorded in history

  Scenario: Export stock report
    Given the inventory has multiple items with different stock levels
    When I navigate to "Stock Control"
    And I click "Export Report"
    And I select format "CSV"
    Then a stock report file should be downloaded
    And the report should contain all current stock data

  # ============================================================================
  # SOFTWARE LICENSE & TOOL TRACKING
  # ============================================================================

  Scenario: Add a software license
    Given I am on the license management page
    When I click "Add License"
    And I fill in the license details:
      | Field              | Value                           |
      | Software Name      | JetBrains IntelliJ IDEA         |
      | License Type       | Annual Subscription             |
      | Number of Seats    | 20                              |
      | Purchase Date      | 2025-01-15                      |
      | Expiration Date    | 2026-01-15                      |
      | License Key        | IJID-2025-XXXX-XXXX-XXXX       |
      | Cost               | 3980.00                         |
    And I click "Save"
    Then the license should be added to the system
    And I should see "License added successfully"

  Scenario: Assign license to user
    Given the following license exists:
      | Software Name | Available Seats |
      | GitHub Copilot | 5              |
    And user "john.doe@company.com" exists in the system
    When I navigate to license assignment
    And I select license "GitHub Copilot"
    And I select user "john.doe@company.com"
    And I click "Assign"
    Then the license should be assigned to the user
    And available seats should decrease to "4"
    And user should receive notification about license assignment

  Scenario: Receive license expiration warning
    Given the following license exists:
      | Software Name    | Expiration Date |
      | Adobe Creative   | 2025-11-20      |
    And today's date is "2025-11-10"
    When I view the dashboard
    Then I should see an expiration warning for "Adobe Creative"
    And the warning should say "Expires in 10 days"

  Scenario: Track tool usage
    Given tool "Portable Hard Drive 2TB" is in inventory
    And the tool is currently available
    When user "jane.smith@company.com" checks out the tool
    And enters purpose "Data backup for client project"
    And sets expected return date to "2025-11-15"
    And confirms checkout
    Then the tool status should change to "In Use"
    And checkout should be recorded in usage history
    And assigned user should be "jane.smith@company.com"

  Scenario: Return a checked-out tool
    Given tool "Oscilloscope" is checked out to "mike.jones@company.com"
    And the checkout date was "2025-11-05"
    When user "mike.jones@company.com" returns the tool
    And enters condition "Good"
    And adds notes "Used for circuit testing"
    And confirms return
    Then the tool status should change to "Available"
    And return should be recorded in usage history
    And total usage days should be calculated

  # ============================================================================
  # USAGE AND MOVEMENT HISTORY
  # ============================================================================

  Scenario: View complete item history
    Given item "ThinkPad X1 Carbon" has multiple transactions
    When I navigate to "ThinkPad X1 Carbon" details
    And I click "View History"
    Then I should see a chronological list of all transactions
    And each transaction should show:
      | Field           |
      | Date/Time       |
      | Transaction Type|
      | Quantity        |
      | User            |
      | Notes           |

  Scenario: Filter history by date range
    Given the system has transactions spanning multiple months
    When I navigate to "Movement History"
    And I set start date to "2025-10-01"
    And I set end date to "2025-10-31"
    And I click "Apply Filter"
    Then I should see only transactions from October 2025
    And transactions outside this range should not be displayed

  Scenario: Filter history by transaction type
    Given the system has various transaction types recorded
    When I navigate to "Movement History"
    And I select transaction type filter "Outgoing"
    And I click "Apply Filter"
    Then I should see only outgoing transactions
    And incoming transactions should be hidden

  Scenario: Track user activity
    Given user "admin@company.com" has performed multiple actions
    When I navigate to "User Activity Log"
    And I search for user "admin@company.com"
    Then I should see all actions performed by this user
    And each action should include timestamp and details

  Scenario: Export movement history report
    Given the system has movement history data
    When I navigate to "Movement History"
    And I set desired filters
    And I click "Export"
    And I select format "PDF"
    Then a detailed movement report should be generated
    And the report should include all filtered transactions

  # ============================================================================
  # USER AUTHENTICATION AND AUTHORIZATION
  # ============================================================================

  Scenario: User login with valid credentials
    Given I am on the login page
    And I am not logged in
    When I enter username "admin@company.com"
    And I enter password "SecurePass123"
    And I click "Login"
    Then I should be logged in successfully
    And I should be redirected to the dashboard
    And I should see "Welcome, Admin"

  Scenario: User login with invalid credentials
    Given I am on the login page
    When I enter username "admin@company.com"
    And I enter password "WrongPassword"
    And I click "Login"
    Then I should see an error "Invalid credentials"
    And I should remain on the login page

  Scenario: Role-based access control
    Given I am logged in as "viewer" role
    When I attempt to access "Add New Item" functionality
    Then I should see "Access Denied: Insufficient permissions"
    And the action should not be executed

  Scenario: User logout
    Given I am logged in
    When I click on user menu
    And I select "Logout"
    Then I should be logged out
    And I should be redirected to the login page
    And my session should be terminated

  # ============================================================================
  # DASHBOARD AND REPORTING
  # ============================================================================

  Scenario: View dashboard summary
    Given I am logged in as administrator
    When I navigate to the dashboard
    Then I should see the following widgets:
      | Widget                    |
      | Total Items in Inventory  |
      | Low Stock Alerts          |
      | Recent Transactions       |
      | License Expiration Alerts |
      | Active Tool Checkouts     |

  Scenario: Generate inventory report
    Given the system has inventory data
    When I navigate to "Reports"
    And I select "Inventory Report"
    And I choose date range "Last 30 days"
    And I click "Generate"
    Then a comprehensive inventory report should be created
    And the report should include summary statistics
    And the report should be available for download

  Scenario: View analytics dashboard
    Given transaction data exists in the system
    When I navigate to "Analytics"
    Then I should see charts for:
      | Chart Type                    |
      | Inventory value over time     |
      | Most frequently used items    |
      | Stock movement trends         |
      | License utilization rate      |

  # ============================================================================
  # DATA VALIDATION AND ERROR HANDLING
  # ============================================================================

  Scenario: Prevent duplicate item entries
    Given item "Logitech MX Master 3" already exists in inventory
    When I try to add a new item with name "Logitech MX Master 3"
    And I click "Save"
    Then I should see an error "Item already exists in inventory"
    And the duplicate item should not be added

  Scenario: Validate required fields
    Given I am adding a new item
    When I leave "Item Name" field empty
    And I click "Save"
    Then I should see validation error "Item Name is required"
    And the item should not be saved

  Scenario: Handle system errors gracefully
    Given a database connection error occurs
    When I attempt to save an item
    Then I should see user-friendly error message "Unable to save. Please try again."
    And the error should be logged for administrators
    And the system should remain stable
