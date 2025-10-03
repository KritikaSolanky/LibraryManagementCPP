# LibraryManagementCPP
Simple console-based Library Management System in C++

# Library Management System (C++)

A simple **console-based Library Management System** implemented in C++ following the given specifications.  

---

## Features

### User Roles
- **Admin**: Access to Maintenance, Transactions, Reports  
- **User**: Access to Transactions, Reports only  

### Maintenance Module (Admin Only)
- Add/Update Book (Book or Movie)  
- Add Membership  
- User Management (New/Existing Users)  

### Transactions Module
- **Issue Book**
  - Book must be available  
  - Issue Date cannot be earlier than today  
  - Return Date default 15 days ahead (editable)  
  - Author auto-populated  
- **Return Book**
  - Book serial mandatory  
  - Author auto-populated  
  - Fine calculation included  
  - Fine Paid confirmation (Yes/No simulated via console input)  
- **Pay Fine**
  - Handles pending fines  
  - Cannot return book until fine is paid  

### Reports Module
- List of available books  
- List of active issues  

### Validations
- Mandatory fields checked  
- Radio buttons simulated via single choice input  
- Checkbox simulated via Yes/No input  
- Password hidden during login  

---

## How to Run

1. **Clone the repository:**
   ```bash
   git clone <YOUR_REPO_URL>
