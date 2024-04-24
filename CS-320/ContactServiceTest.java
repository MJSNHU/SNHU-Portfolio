package test;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNull;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

import contact.Contact;
import contact.ContactService;

@TestInstance(Lifecycle.PER_CLASS)
public class ContactServiceTest {
    
	 private ContactService cs = new ContactService();
	
	// populate contacts array with three items
	@BeforeAll
	public void setup() {
   	   
        cs.addContact("0123456789", "Matt", "Jackson", "1111111111", "123 Any Street, Manchester, NH");
        cs.addContact(new Contact("1234567890", "Mason", "Jandreau", "1231234567", "333 Anystreet, Manchester, NH"));
        cs.addContact("IDTODELETE");
	}
	
	
	@Test
	public void testAddContact() {
		Contact c = new Contact("0123456780", "Mark", "Jacobs", "1111111112", "122 Any Street, Manchester, NH");
		cs.addContact(c);
		assertTrue(c.getID().equals("0123456780"));
	}
	@Test
	public void testAddContactInvalidID() {
    Assertions.assertThrows(IllegalArgumentException.class, () -> {
    		cs.addContact("01234567890");
    	});
	}
	
   	@Test
	public void testAddDuplicateId() {
		Contact c = new Contact("0123456789", "Mike", "Johnson", "1234567890", "111 Any Street, Manchester, NH");
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			cs.addContact(c);
		});
	}
   	
   	@Test
   	public void testAddDuplicateIdOnly() {
   		Contact c = new Contact("0123456789");
   		Assertions.assertThrows(IllegalArgumentException.class, () -> {
   			cs.addContact(c);
   		});
  	}
   	
   	// delete Contact by ID,
   	// then check each contact ID to ensure it's gone
   	@Test
   	public void testDeleteContact() {
   		cs.deleteContact("IDTODELETE");
   		Contact c = cs.getContact("IDTODELTE");
   		assertNull(c);
   		
   	}
   	
   	@Test
   	public void testUpdateFirstName() {
   		String ID = "0123456789";
   		cs.updateFirstName(ID, "Mick");
   		Contact c = cs.getContact(ID);
   		assertTrue(c.getFirstName().equals("Mick"));
   	}

   	@Test
   	public void testUpdateLastName() {
   		String ID = "0123456789";
   		cs.updateLastName(ID, "Jenkins");
   		Contact c = cs.getContact(ID);
   		assertTrue(c.getLastName().equals("Jenkins"));
   	}
   	
   	@Test
   	public void testUpdateNumber() {
   		String ID = "0123456789";
   		cs.updateNumber(ID, "2223334444");
   		Contact c = cs.getContact(ID);
   		assertTrue(c.getNumber().equals("2223334444"));
   	}
   	
   	@Test
   	public void testUpdateAddress() {
   		String ID = "0123456789";
		cs.updateAddress(ID, "555 Any Street, Manchester, NH");
		Contact c = cs.getContact(ID);
		assertTrue(c.getAddress().equals("555 Any Street, Manchester, NH"));
	}
   	
}
