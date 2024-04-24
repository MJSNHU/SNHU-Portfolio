package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.Test;
import org.junit.jupiter.api.Assertions;

import contact.Contact;

public class ContactTest {

	Contact contact = new Contact("1", "Matt", "Jackson", "1111111111", "123 Any Street, Manchester, NH");
	
	@Test
	public void testContact() {
		
		assertTrue(contact.getID().equals("1"));
		assertTrue(contact.getFirstName().equals("Matt"));
        assertTrue(contact.getLastName().equals("Jackson"));
        assertTrue(contact.getNumber().equals("1111111111"));
        assertTrue(contact.getAddress().equals("123 Any Street, Manchester, NH"));
        
	}
	
	@Test
	public void testContactIDTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01234567890", "Matt", "Jackson", "1111111111", "123 Any Street, Manchester, NH");
		});
	}
	
	@Test 
	public void testContactIDNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact(null, "firstName", "lastName", "number", "address");
		});
	}
	
	@Test
	public void testContactIDOnlyNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact(null);
		});
	}
	
	@Test
	public void testContactFirstNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456789", "Matt4567890", "Jackson", "1111111111", "123 Any Street, Manchester, NH");
		});
	}
	
	@Test 
	public void testContactFirstNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("2", null, "lastName", "number", "address");
		});
	}
	
	@Test
	public void testContactLastNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456788", "Matt", "Jackson7890", "1111111111", "123 Any Street, Manchester, NH");
		});
	}
	
	@Test
	public void testContactLastNameNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("2", "firstName", null, "number", "address");
		});
	}

	@Test
	public void testContactPhoneNot10() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456787", "Matt", "Jackson", "111111111", "123 Any Street, Manchester, NH");
		});
	}
	
	@Test
	public void testContactPhoneNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456786", "Matt", "Jackson", null, "123 Any Street, Manchester, NH");
		});
	}
	
	@Test
	public void testContactAddressTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456785", "Matt", "Jackson", "1111111111", "123 Any Street, Manchester, New Hampshire");
		});
	}
	
	@Test
	public void testContactAddressNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("0123456785", "Matt", "Jackson", "1111111111", null);
		});
	}
	
	@Test
	public void testSetFirstName() {
		contact.setFirstName("Mick");
		assertTrue(contact.getFirstName().equals("Mick"));
	}
	
	@Test
	public void testSetLastName() {
		contact.setLastName("Jamieson");
	    assertTrue(contact.getLastName().equals("Jamieson"));
	}
	
	@Test
	public void testSetNumber() {
		contact.setNumber("1112223333");
		assertTrue(contact.getNumber().equals("1112223333"));
	}
	
	@Test
	public void testSetAddress() {
		contact.setAddress("555 Any Street, Manchester, NH");
		assertTrue(contact.getAddress().equals("555 Any Street, Manchester, NH"));
	}
}
