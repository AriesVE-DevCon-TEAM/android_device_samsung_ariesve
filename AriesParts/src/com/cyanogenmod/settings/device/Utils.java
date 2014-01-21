/*
 * Copyright (C) 2013 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device;

import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;

/**
 * Utility class
 */
public class Utils {

	private static final String TAG_READ = "DeviceSettings_Utils_Read";
	private static final String TAG_WRITE = "DeviceSettings_Utils_Write";

	/**
	 * Checks if the specified file path exists
	 * @param path The file path to check
	 * @return True if the path exists
	 */
	public static boolean fileExists(String path) {
		return new File(path).exists();
	}

	/**
	 * Checks if the specified path exists and is a directory
	 * @param path The directory path to check
	 * @return True if the path exists and is a directory
	 */
	public static boolean directoryExists(String path) {
		File directory = new File(path);
		return directory.exists() && directory.isDirectory();
	}

	/**
	 * Reads from a file
	 * @param path The file path
	 * @return The content of the file, if found, otherwise a string empty
	 */
	public static String readValue(String path) {
		// Initialization of the content to return
		StringBuilder content = new StringBuilder();

		try {
			// Create a new buffered reader to read the file content
			BufferedReader reader = new BufferedReader(new FileReader(new File(path)));

			// Read each file line till the end and append it to the content StringBuilder
			String line = null;
			while ((line = reader.readLine()) != null)
				content.append(line);

			// Close the file
			reader.close();
		} catch (FileNotFoundException ex) {
			Log.e(TAG_READ, String.format("The file %s not exists", path));
		} catch (IOException ex) {
			Log.e(TAG_READ, String.format("Error during file %s reading", path));
		}

		// Return the content of the file
		return content.toString();
	}

	/**
	 * Writes the specified text value to the file
	 * @param path File path
	 * @param value The value to write
	 * @return The result of the operation (true = successful write, false = write failed)
	 */
	public static boolean writeValue(String path, String value) {
		// If the value is null, then replace it with an empty string
		if (value == null)
			value = "";

		try {
			// Create the output stream to perform file writes
			FileOutputStream stream = new FileOutputStream(new File(path));

			// Write the passed value
			stream.write(value.getBytes());

			// Flush and close the file
			stream.flush();
			stream.close();

			// Successful write
			return true;
		} catch (FileNotFoundException e) {
			Log.e(TAG_WRITE, String.format("The file %s not exists", path));
			return false;
		} catch (IOException e) {
			Log.e(TAG_WRITE, String.format("Error during file %s writing", path));
			return false;
		}
	}

}
